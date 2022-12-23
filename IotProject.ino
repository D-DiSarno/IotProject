#include <SPI.h>

#include <WiFi.h>
#include "WiFiClientSecure.h"

#include "Cipher.h"

#include <MFRC522.h>

#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"

#define SS_PIN 5    // ESP32 pin GIOP5
#define RST_PIN 27  // ESP32 pin GIOP27

const char* ssid = "hotspot_iot";
const char* password = "hotspot_iot";

String card1 = "FFFFFFFF";  // Change this value to the UID of your card.
MFRC522 mfrc522(SS_PIN, RST_PIN);

Cipher* cipher = new Cipher();

const char* hostname = "example.org";
unsigned int port = 11111;
WiFiClientSecure client(hostname);

int getUserPassword(String);
int createUser(String, String);
int addPassword(String, String, String, String);
int deletePassword(String, String, String);

const char* test_root_ca = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIEYDCCA0igAwIBAgIJAPj0cj2xf4dFMA0GCSqGSIb3DQEBCwUAMIGGMQswCQYD\n" \
  "VQQGEwJJVDEOMAwGA1UECAwFSXRhbHkxEDAOBgNVBAcMB1NhbGVybm8xDDAKBgNV\n" \
  "BAoMA0lvVDEMMAoGA1UECwwDSW9UMQ8wDQYDVQQDDAZkb21haW4xKDAmBgkqhkiG\n" \
  "9w0BCQEWGWRhdmlkZS5kaXNhcm5vQGljbG91ZC5jb20wHhcNMjIxMjIxMjEwNDI3\n" \
  "WhcNMjMxMjIxMjEwNDI3WjCBhjELMAkGA1UEBhMCSVQxDjAMBgNVBAgMBUl0YWx5\n" \
  "MRAwDgYDVQQHDAdTYWxlcm5vMQwwCgYDVQQKDANJb1QxDDAKBgNVBAsMA0lvVDEP\n" \
  "MA0GA1UEAwwGZG9tYWluMSgwJgYJKoZIhvcNAQkBFhlkYXZpZGUuZGlzYXJub0Bp\n" \
  "Y2xvdWQuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqegGqHvB\n" \
  "589r9JJIR83Y2zctA1WbOWUUw+NJnrRWxqCZ58baVILt8cGnM/VXRPO6P493HNSL\n" \
  "XjaEzqIS81yv2SMZbv3FbGqyx/D/L2TXyX8GTHHgLI53U0wQqYyDOl+0S6RKPDRu\n" \
  "nTbAbA40PRHqRWHLSnyOjWXDt6jL4Q2vbFwf4OxP1IE/noNcy3lUSbH5IaZv2oDT\n" \
  "zJQmVy/iCrObkJk3niNRtKftRbK+K7CjO7iuCo26qTjkMXL90GAAm1JmQQbOvPDW\n" \
  "knACndJ4eJy7uLU0YJr+nO1sjT81QXL46Y9/1lv/pXaj4wXmsAPaBk17qb+g6qiM\n" \
  "QuXCJTfWD7lW1wIDAQABo4HOMIHLMIGlBgNVHSMEgZ0wgZqhgYykgYkwgYYxCzAJ\n" \
  "BgNVBAYTAklUMQ4wDAYDVQQIDAVJdGFseTEQMA4GA1UEBwwHU2FsZXJubzEMMAoG\n" \
  "A1UECgwDSW9UMQwwCgYDVQQLDANJb1QxDzANBgNVBAMMBmRvbWFpbjEoMCYGCSqG\n" \
  "SIb3DQEJARYZZGF2aWRlLmRpc2Fybm9AaWNsb3VkLmNvbYIJAMetDkBGmzrDMAkG\n" \
  "A1UdEwQCMAAwFgYDVR0RBA8wDYILZXhhbXBsZS5vcmcwDQYJKoZIhvcNAQELBQAD\n" \
  "ggEBAFVZ1rvb5SCJ8gW0wPcxmAOH9G2WIR7d0ynWJ0bGdQjN7EIaQDFPIpBCfpl+\n" \
  "dHeVz+IsBeq1kN31t7HzfMpLCKKkgwOgpuvICabMc/yXMaSV+Cbq8GBC4RtgwaGg\n" \
  "zId8UA6ChYcXVkCFpkWt8NYVkeMgrgnEni4LXXNEAZoOOa0zvTL3gIM7LwmFz0Te\n" \
  "TWH19zzJ2Q2HEvykU3swqMiWrztpxwjKXdHy2zyrG8slQs07kDjUCjrMmAPpDnHK\n" \
  "7SyiNlh3RDaSRD2C8C7UwRFnjxmg7r3OIFw5QCyp9hEA07tVudIVACAIlJbHUhAH\n" \
  "20lSNGmmZqVqdt2T+VpLpT8IlU8=\n" \
  "-----END CERTIFICATE-----\n";

// You can use x.509 client certificates if you want
const char* test_client_key = \
  "-----BEGIN RSA PRIVATE KEY-----\n"
  "MIIEpAIBAAKCAQEAqegGqHvB589r9JJIR83Y2zctA1WbOWUUw+NJnrRWxqCZ58ba\n" \
  "VILt8cGnM/VXRPO6P493HNSLXjaEzqIS81yv2SMZbv3FbGqyx/D/L2TXyX8GTHHg\n" \
  "LI53U0wQqYyDOl+0S6RKPDRunTbAbA40PRHqRWHLSnyOjWXDt6jL4Q2vbFwf4OxP\n" \
  "1IE/noNcy3lUSbH5IaZv2oDTzJQmVy/iCrObkJk3niNRtKftRbK+K7CjO7iuCo26\n" \
  "qTjkMXL90GAAm1JmQQbOvPDWknACndJ4eJy7uLU0YJr+nO1sjT81QXL46Y9/1lv/\n" \
  "pXaj4wXmsAPaBk17qb+g6qiMQuXCJTfWD7lW1wIDAQABAoIBAQCTrEAFNjNNiGGb\n" \
  "G31JM2eU+Lrd2Azm8K0AqM626qU1VCcQZSwkIfwvkJIDveyQ2Il7bvUnELDlPNtn\n" \
  "QUTZUijgv/n+FwD9fytOTm9tQOewkP/nd15RO64IrucYhjEUjlQFHpELDMNLT2gl\n" \
  "Qp8yqRyGGz52yhWYXeei/EnfnGZo9aqKobFt2XvFC6hTwesafkhPtUHL5CM5CGAc\n" \
  "EMCrRH9xeAW5aK70+z4E81PFx7YfJ/Jaw3CUpwYk/rmVXLq2hJhs5M+GdlG/RbTa\n" \
  "Teul3X+PJcCkvBaevKSHWOa+VLlSu9TvuHazsK/pnOLm6aL/sqRHuBYNVundVrik\n" \
  "Op7R22eRAoGBANMxS7vs6VBwQIo3+a8fSWPEKff6bt/2+HXhPlkjncPXECXjLlFA\n" \
  "FAU2MfEzOdT13II/IpggRmnv4GbUFDn2J97/4mDxLHp0i2Rh7BsMt60x6GL8ymxc\n" \
  "6mgh7lvrQhVrkMhOyqZR36ZC2rTys0G83ovdflaq2m932UUFt+fupDb5AoGBAM30\n" \
  "UAj6k+wvZER6WO6PbgNxtmbtNQ+7+XZY7vJ/CXZQvDRe1eGvNH4PwfuYDyGxNxRc\n" \
  "0/6TW5Y0xNjToVDRIewIUwKtcdWhjlzlUEqXXzpccJrRYMhrsNQoz8pHm/5yar5T\n" \
  "s6N5DE6rikvCCAsyGJZ3rNOFdT7mN/085VJKp2BPAoGABdh5diwqkRVqU+kUuiot\n" \
  "h9toJfsCkqRtYCXp6eG4fjE3uzS8lXcmlrZXT4vRb/mWLuy8cmWCTXBcT7xTCv+r\n" \
  "PtrqUzCZwJH1QcQMGvuRIfQdsI+6sqEnw9YcmOK8I4ekrEayCUR+4kU7dIK7P3iQ\n" \
  "2GI7Gv8DqM4yb1ynKH5MgVECgYEAjMyturW0Uw/DOJ18FvuvdmNTE+KvF+amkSYT\n" \
  "lW0MBvsC77q19ehVtqkHMP2vupTw1tbvh2cuge3pdWKZMisHSdoXBgmVfTtJnHCZ\n" \
  "B6Wc0MVJpk3mIJ6e8SxFgkCagMWv/BY1BVlH9LCJh2lLHnxNGvEOEj12/l1dgZ8U\n" \
  "F8I50zkCgYBrOXk9TZA+NSvMx3SUTaAqYTPwE28uu645DeCjwMgPxE6FWebHo4zx\n" \
  "Pzgd72xrnPxGvip2CMx8NCjCvpD1SgubEpnTOQZWUs0cytsfZCrvTlA/89siYU8j\n" \
  "TQ4UAnIk3CTUAcDZ8XQ/+mOZD0p19gvmF7qOiYfNxo7VGktRakjQDQ==\n" \
  "-----END RSA PRIVATE KEY-----\n";

const char* test_client_cert = \
    "-----BEGIN CERTIFICATE-----\n" \
  "MIIEYDCCA0igAwIBAgIJAPj0cj2xf4dFMA0GCSqGSIb3DQEBCwUAMIGGMQswCQYD\n" \
  "VQQGEwJJVDEOMAwGA1UECAwFSXRhbHkxEDAOBgNVBAcMB1NhbGVybm8xDDAKBgNV\n" \
  "BAoMA0lvVDEMMAoGA1UECwwDSW9UMQ8wDQYDVQQDDAZkb21haW4xKDAmBgkqhkiG\n" \
  "9w0BCQEWGWRhdmlkZS5kaXNhcm5vQGljbG91ZC5jb20wHhcNMjIxMjIxMjEwNDI3\n" \
  "WhcNMjMxMjIxMjEwNDI3WjCBhjELMAkGA1UEBhMCSVQxDjAMBgNVBAgMBUl0YWx5\n" \
  "MRAwDgYDVQQHDAdTYWxlcm5vMQwwCgYDVQQKDANJb1QxDDAKBgNVBAsMA0lvVDEP\n" \
  "MA0GA1UEAwwGZG9tYWluMSgwJgYJKoZIhvcNAQkBFhlkYXZpZGUuZGlzYXJub0Bp\n" \
  "Y2xvdWQuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqegGqHvB\n" \
  "589r9JJIR83Y2zctA1WbOWUUw+NJnrRWxqCZ58baVILt8cGnM/VXRPO6P493HNSL\n" \
  "XjaEzqIS81yv2SMZbv3FbGqyx/D/L2TXyX8GTHHgLI53U0wQqYyDOl+0S6RKPDRu\n" \
  "nTbAbA40PRHqRWHLSnyOjWXDt6jL4Q2vbFwf4OxP1IE/noNcy3lUSbH5IaZv2oDT\n" \
  "zJQmVy/iCrObkJk3niNRtKftRbK+K7CjO7iuCo26qTjkMXL90GAAm1JmQQbOvPDW\n" \
  "knACndJ4eJy7uLU0YJr+nO1sjT81QXL46Y9/1lv/pXaj4wXmsAPaBk17qb+g6qiM\n" \
  "QuXCJTfWD7lW1wIDAQABo4HOMIHLMIGlBgNVHSMEgZ0wgZqhgYykgYkwgYYxCzAJ\n" \
  "BgNVBAYTAklUMQ4wDAYDVQQIDAVJdGFseTEQMA4GA1UEBwwHU2FsZXJubzEMMAoG\n" \
  "A1UECgwDSW9UMQwwCgYDVQQLDANJb1QxDzANBgNVBAMMBmRvbWFpbjEoMCYGCSqG\n" \
  "SIb3DQEJARYZZGF2aWRlLmRpc2Fybm9AaWNsb3VkLmNvbYIJAMetDkBGmzrDMAkG\n" \
  "A1UdEwQCMAAwFgYDVR0RBA8wDYILZXhhbXBsZS5vcmcwDQYJKoZIhvcNAQELBQAD\n" \
  "ggEBAFVZ1rvb5SCJ8gW0wPcxmAOH9G2WIR7d0ynWJ0bGdQjN7EIaQDFPIpBCfpl+\n" \
  "dHeVz+IsBeq1kN31t7HzfMpLCKKkgwOgpuvICabMc/yXMaSV+Cbq8GBC4RtgwaGg\n" \
  "zId8UA6ChYcXVkCFpkWt8NYVkeMgrgnEni4LXXNEAZoOOa0zvTL3gIM7LwmFz0Te\n" \
  "TWH19zzJ2Q2HEvykU3swqMiWrztpxwjKXdHy2zyrG8slQs07kDjUCjrMmAPpDnHK\n" \
  "7SyiNlh3RDaSRD2C8C7UwRFnjxmg7r3OIFw5QCyp9hEA07tVudIVACAIlJbHUhAH\n" \
  "20lSNGmmZqVqdt2T+VpLpT8IlU8=\n" \
  "-----END CERTIFICATE-----\n";
