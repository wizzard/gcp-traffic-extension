# gcp-traffic-extension
A simple GCP traffic extension in C++

## Local test with Envoy

* requires Bazel v7.4 (not yet supported Bzlmod)
* run `./build_local.sh`
* generate certs: `openssl req -x509 -nodes -days 36500 -newkey rsa:2048 -keyout server.key -out server.crt -subj "/C=US/ST=State/L=City/O=Organization/OU=Unit/CN=example.com" && cat server.crt server.key > server.pem`
* run `./bazel-bin/src/gcp-traffic-extension --cert_dir .`
* adjust enovy `ext_proc` settings

## GCP LB test

* follow the guides:

1. [Configure a callout backend service](https://cloud.google.com/service-extensions/docs/configure-callout-backend-service)
2. [Configure a traffic extension](https://cloud.google.com/service-extensions/docs/configure-traffic-extensions)

* Dokerize and publish an image, use it instead `service-extensions-samples`
* adjust and import `traffic-ext-test.yaml`:

```bash
gcloud service-extensions lb-traffic-extensions import [NAME] \
    --source=traffic-ext-test.yaml \
    --location=[REGION]
```

## Sending requests

1. To receive request headers and body: `curl http://[your server]/test-body --data 'THIS IS THE TEST BODY' -X POST`
2. To receive only request headers: `curl http://[your server]/`