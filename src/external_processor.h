#ifndef EXTERNAL_PROCESSOR_H
#define EXTERNAL_PROCESSOR_H

#include "envoy/service/ext_proc/v3/external_processor.grpc.pb.h"

using envoy::service::ext_proc::v3::ExternalProcessor;
using envoy::service::ext_proc::v3::ProcessingRequest;
using envoy::service::ext_proc::v3::ProcessingResponse;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

class TestExternalProcessor final : public ExternalProcessor::Service
{
public:
    TestExternalProcessor(bool debug) : ExternalProcessor::Service(), debug(debug) {}
    Status Process(ServerContext* context, ServerReaderWriter<ProcessingResponse, ProcessingRequest>* stream);
private:
    bool debug;
};

#endif // EXTERNAL_PROCESSOR_H