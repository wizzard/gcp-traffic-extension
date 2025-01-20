load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_proto_grpc",
    integrity = "sha256-wNcY9NiSxSQCVQTmelv+gzYLOpguZUvHH+11FOuKyK0=",
    strip_prefix = "rules_proto_grpc-4.6.0",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/4.6.0.tar.gz"],
)

http_archive(
    name = "data_plane_api",
    strip_prefix = "data-plane-api-6de09125b3c9b64bbd78dd8c5f9503d0cb5509da",
    urls = [
        "https://github.com/envoyproxy/data-plane-api/archive/6de09125b3c9b64bbd78dd8c5f9503d0cb5509da.tar.gz",
    ],
)

http_archive(
    name = "com_github_cncf_xds",
    strip_prefix = "xds-cff3c89139a3e6a0d4fbddfd158ad895e9b30840",
    urls = [
        "https://github.com/cncf/xds/archive/cff3c89139a3e6a0d4fbddfd158ad895e9b30840.tar.gz"
    ]
)

http_archive(
    name = "build_bazel_rules_apple",
    sha256 = "34c41bfb59cdaea29ac2df5a2fa79e5add609c71bb303b2ebb10985f93fa20e7",
    url = "https://github.com/bazelbuild/rules_apple/releases/download/3.1.1/rules_apple.3.1.1.tar.gz",
)

load("@build_bazel_rules_apple//apple:repositories.bzl", "apple_rules_dependencies",)
apple_rules_dependencies()



load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_toolchains", "rules_proto_grpc_repos")
rules_proto_grpc_toolchains()
rules_proto_grpc_repos()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_repos", "rules_proto_grpc_toolchains")
rules_proto_grpc_toolchains()
rules_proto_grpc_repos()

load("@rules_proto_grpc//cpp:repositories.bzl", rules_proto_grpc_cpp_repos = "cpp_repos")
rules_proto_grpc_cpp_repos()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()


http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/releases/download/20240722.0/abseil-cpp-20240722.0.tar.gz"],
  strip_prefix = "abseil-cpp-20240722.0",
)


