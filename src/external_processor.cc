#include "absl/log/initialize.h"
#include "absl/log/globals.h"
#include "absl/log/log.h"

#include "external_processor.h"

using ::envoy::config::core::v3::HeaderMap;
using ::envoy::config::core::v3::HeaderValue;
using envoy::extensions::filters::http::ext_proc::v3::ProcessingMode;
using envoy::service::ext_proc::v3::HttpHeaders;

#define HDR_PATH ":path"
#define HDR_METHOD ":method"
#define HDR_AUTHORITY ":authority"
#define HDR_SCHEME ":scheme"

#define TEST_BODY_ENDPOINT "/test-body"

Status TestExternalProcessor::Process(ServerContext *context, ServerReaderWriter<ProcessingResponse, ProcessingRequest> *stream)
{
    ProcessingRequest req;

    while (stream->Read(&req)) {
        switch (req.request_case()) {
        case ProcessingRequest::kRequestHeaders:
        {
            HeaderMap headers = req.request_headers().headers();
            bool found = false;
            std::string path;
            std::string method;

            LOG(INFO) << ">>> Request HEADERS received. Total " << headers.headers_size() << " headers";

            for (int i = 0; i < headers.headers_size(); i++) {
                HeaderValue v = headers.headers(i);
                const char *key = v.key().c_str();

                if (!key)
                    continue;

                if (debug)
                    LOG(INFO) << "\t" << key << " : " << v.raw_value();

                if (!strcmp(key, HDR_PATH)) {
                    if (v.raw_value() == TEST_BODY_ENDPOINT) {
                        found = true;
                    }
                    path = v.raw_value();
                }  else if (!strcmp(key, HDR_METHOD)) {
                    method = v.raw_value();
                }
            }

            ProcessingResponse header_resp;
            header_resp.mutable_request_headers();
            if (found) {
                LOG(INFO) << "enabling ProcessingMode::STREAMED";

                // Enable kRequestBody event
                auto *override = header_resp.mutable_mode_override();

                // setting STREAMED mode
                override->set_request_body_mode(ProcessingMode::STREAMED);

                // XXX: seems like this is not working
                //override->set_request_body_mode(ProcessingMode::BUFFERED);
            }

            stream->Write(header_resp);

            break;
        }

        case ProcessingRequest::kRequestBody:
        {
            LOG(INFO) << ">>> Request BODY received. Body size: " << req.request_body().body().size() << " bytes";
            if (debug)
                LOG(INFO)<< ">>> Request BODY: " << req.request_body().body();

            ProcessingResponse body_resp;
            body_resp.mutable_request_body();

            // disable any further events
            auto *mode_override = body_resp.mutable_mode_override();
            mode_override->set_request_body_mode(ProcessingMode::NONE);

            stream->Write(body_resp);

            break;
        }

        case ProcessingRequest::kRequestTrailers:
        {
            LOG(INFO) << ">>> Request TRAILERS received";
            break;
        }

        case ProcessingRequest::kResponseHeaders:
        {
            HeaderMap headers = req.response_headers().headers();

            LOG(INFO) << "<<< Response HEADERS received. Total " << headers.headers_size() << " headers";

            for (int i = 0; i < headers.headers_size(); i++) {
                HeaderValue v = headers.headers(i);
                const char *key = v.key().c_str();

                if (!key)
                    continue;

                if (debug)
                    LOG(INFO) << "\t" << key << " : " << v.raw_value();
            }

            ProcessingResponse header_resp;
            header_resp.mutable_response_headers();
            stream->Write(header_resp);

            break;
        }

        case ProcessingRequest::kResponseBody:
        {
            LOG(INFO) << "<<< Response BODY received. Body size: " << req.request_body().body().size() << " bytes";
            break;
        }

        default:
            LOG(INFO) << "DEFAULT";
            break;
        }
    }

    return Status::OK;
}
