# gcp-traffic-extension
A simple GCP traffic extension in C++

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

## Local test with Envoy

* execute `docker-compose build && docker-compose up`
* Envoy will listen on 127.0.0.1:8080


## Sending requests

1. To receive request headers and body: `curl http://[your server]/test-body --data 'THIS IS THE TEST BODY' -X POST`
2. To receive only request headers: `curl http://[your server]/`