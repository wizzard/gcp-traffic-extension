FROM ubuntu:22.04 AS builder

RUN apt update && \
    apt install --no-install-recommends -y wget build-essential ca-certificates

RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.25.0/bazelisk-linux-amd64 && \
    chmod 755 bazelisk-linux-amd64 && \
    mv bazelisk-linux-amd64 /usr/bin/bazel

WORKDIR /build
COPY WORKSPACE /build/
COPY .bazelrc /build/
COPY src /build/src

RUN USE_BAZEL_VERSION=7.4.1 /usr/bin/bazel build //src:gcp-traffic-extension --compilation_mode=fastbuild


FROM alpine:latest AS runner

RUN apk add --no-cache openssl && \
    rm -rf /var/cache/apk/*

COPY --from=builder /build/bazel-bin/src/gcp-traffic-extension /app/gcp-traffic-extension
RUN mkdir /etc/certs && cd /etc/certs && \
    openssl req -x509 -nodes -days 36500 -newkey rsa:2048 -keyout server.key -out server.crt -subj "/C=US/ST=State/L=City/O=Organization/OU=Unit/CN=example.com" && \
     cat server.crt server.key > server.pem

RUN addgroup -S traffic_extension && adduser -S traffic_extension -G traffic_extension && chown -R traffic_extension /etc/certs

WORKDIR /app

USER traffic_extension

EXPOSE 50051

CMD ["/app/gcp-traffic-extension", "--cert_dir", "/etc/certs"]