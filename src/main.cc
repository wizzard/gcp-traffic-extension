#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/initialize.h"
#include "absl/log/globals.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"

#include <filesystem>
#include <grpc++/grpc++.h>
#include <grpc++/health_check_service_interface.h>
#include <grpc++/security/server_credentials.h>

#include "external_processor.h"
#include "utils.h"

using grpc::AuthContext;
using grpc::AuthProperty;
using grpc::Server;
using grpc::ServerBuilder;

const char *DEFAULT_ADDR = "0.0.0.0";
int DEFAULT_PORT = 50051;
const char *DEFAULT_CERT_DIR = "/etc/cert";
const char *MOD_NAME = "Traffic Extension Server";
const char *CERT_KEY_FILE = "server.key";
const char *CERT_FILE = "server.crt";
const grpc::string hc_service_name("health_check");

ABSL_FLAG(std::string, address, DEFAULT_ADDR, "Server address");
ABSL_FLAG(uint16_t, port, DEFAULT_PORT, "Server port");
ABSL_FLAG(bool, debug, false, "Debug mode");
ABSL_FLAG(std::string, cert_dir, DEFAULT_CERT_DIR, "Full path to the certificate directory");

int main(int argc, char** argv)
{
    absl::InitializeLog();
    absl::SetProgramUsageMessage(MOD_NAME);
    absl::ParseCommandLine(argc, argv);

    std::string addr = absl::StrFormat("%s:%d",
        absl::GetFlag(FLAGS_address),
        absl::GetFlag(FLAGS_port));


    absl::SetMinLogLevel(absl::LogSeverityAtLeast::kInfo);
    absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);

    TestExternalProcessor service(absl::GetFlag(FLAGS_debug));

    std::filesystem::path full_path = absl::GetFlag(FLAGS_cert_dir);

    std::string key_file = full_path / CERT_KEY_FILE;
    std::string cert_file = full_path / CERT_FILE;

    grpc::SslServerCredentialsOptions ssl_opts;
    ssl_opts.pem_key_cert_pairs.push_back(
        {read_text_file(key_file), read_text_file(cert_file)});

    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::SslServerCredentials(ssl_opts));
    // Keep-Alive
    builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIME_MS,
        10 * 60 * 1000 /*10 min*/);
    builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIMEOUT_MS,
        20 * 1000 /*20 sec*/);
    builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);
    builder.AddChannelArgument(
        GRPC_ARG_HTTP2_MIN_RECV_PING_INTERVAL_WITHOUT_DATA_MS,
        10 * 1000 /*10 sec*/);

    grpc::EnableDefaultHealthCheckService(true);

    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG(INFO) << "Starting Traffic Extension Server on " << addr;

    // HealthCheck
    grpc::HealthCheckServiceInterface *hc = server->GetHealthCheckService();
    hc->SetServingStatus(hc_service_name, true);

    server->Wait();

    return 0;
}