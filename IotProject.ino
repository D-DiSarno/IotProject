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

int getUserPassword(String);
int createUser(String, String);
int addPassword(String, String, String, String);
int deletePassword(String, String, String);



const char* hostname = "example.org";
unsigned int port = 11111;
WiFiClientSecure client(hostname);

const char* test_root_ca = \
<<<<<<< HEAD
                           "-----BEGIN CERTIFICATE-----\n" \
                           "MIID7zCCAtegAwIBAgIUb1aKD6PrUPRLWFASn7z7vKnHHnMwDQYJKoZIhvcNAQEL\n" \
                           "BQAwgYYxCzAJBgNVBAYTAml0MQswCQYDVQQIDAJpdDELMAkGA1UEBwwCaXQxFDAS\n" \
                           "BgNVBAoMC2V4YW1wbGUub3JnMRQwEgYDVQQLDAtleGFtcGxlLm9yZzEUMBIGA1UE\n" \
                           "AwwLZXhhbXBsZS5vcmcxGzAZBgkqhkiG9w0BCQEWDGdhQGdtYWlsLmNvbTAeFw0y\n" \
                           "MjEyMDkyMDUzNTVaFw0yNzEyMDgyMDUzNTVaMIGGMQswCQYDVQQGEwJpdDELMAkG\n" \
                           "A1UECAwCaXQxCzAJBgNVBAcMAml0MRQwEgYDVQQKDAtleGFtcGxlLm9yZzEUMBIG\n" \
                           "A1UECwwLZXhhbXBsZS5vcmcxFDASBgNVBAMMC2V4YW1wbGUub3JnMRswGQYJKoZI\n" \
                           "hvcNAQkBFgxnYUBnbWFpbC5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
                           "AoIBAQDT4c/z2dmTiBdkEHnkTcg/z7zB9zH3riBlBVbk/l3rNjbA87eEyTg6gi21\n" \
                           "41huAd6R4JzcJvvSPSeIJjcbn85NQXF9Ws77DA/5HpvQa4nIn8jM67XjrKD78E5S\n" \
                           "Tmblq9n5o7HSDc/3kRrpMXuGmFfitQGYgXuvqH4126J+TK48TPYczinYIehKKnDC\n" \
                           "olKFLTl3V2eJGCDlOcrtn1QmGYZ6Af2Pzg8DzAPG4J3O0jgH/FN88HsNwCAl1rc8\n" \
                           "NQyj5/uZGKbFhxBryTNz83IKFZ3Y6ctijGWQ5SC3iJStO9zxnndhUryba6l7cImJ\n" \
                           "cfKNA8a4MeIo2KmBNa1ZLeTSSu0rAgMBAAGjUzBRMB0GA1UdDgQWBBRWc57qbriI\n" \
                           "1G3uRQ40b0913a7rkDAfBgNVHSMEGDAWgBRWc57qbriI1G3uRQ40b0913a7rkDAP\n" \
                           "BgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQA46kR7NNPgaHJ1YEGq\n" \
                           "exnczfyf8KvMXKoynTTNDRMp1JfxAArPFigDyO58/CBokvtM9TRPeYHu5cI+Pa4P\n" \
                           "P2cgxsYWfIqf9qncCQ1NOsL0LSpH576ats+PCdEzIIL+ypUDLBUdSllLsDfjq0oH\n" \
                           "UyekWV55guFhNmk3IyjlUgDLtmf9Aeq2XS/yawxNADEKnlQcNc2fFQR/wXLi8p4F\n" \
                           "p6IqM3V2n3nVlsu25y01sU9nhukRF3kl/VBek8tIyl9VHcEzAWHn+q5QmamyUSHK\n" \
                           "X2u/5OGaFjCrR7hnzlQZSoF3DAURUCU064Eync6acAIMqssipgCjwg6MHTuWJo7/\n" \
                           "JkPm\n" \
                           "-----END CERTIFICATE-----\n";


// You can use x.509 client certificates if you want
const char* test_client_key = \
                              "-----BEGIN RSA PRIVATE KEY-----\n" \
                              "MIIEpQIBAAKCAQEA21VzTHsxzVYiJyzXp1nUXV9KJPPa0zxLGXi1u7X+JSroHby5\n" \
                              "gpckZRfcZ8A6QmiU6oSV7Kw/LvGR0VjuFrPmyq2UY35c67jcuFgtqr0lcNueFFZE\n" \
                              "5VsJZYvEBwyQt1PEPcKAOU5vv6BgTwAWAoR2fnLen5+MLs+Pg6nh4tb4JBTji+n6\n" \
                              "vWoKrjjV16AQYtpmmO7yYU6AdkOWrUuBcSiIuctnROHa9nFuieY42R05HO1nuPnH\n" \
                              "SSCtnKf5DFQtS3xIAxvKhUCONxoPZ+dEpCVK3bQhPtShKyvY2cKti6CEFFJKP+/H\n" \
                              "UfrJWmlm5Cl37MV9f6FcogEpPHV31PFvuAYBwwIDAQABAoIBACyWm4DhzocKypii\n" \
                              "MhkQVu7Ab2dBt6uXp0C/XFqTrd3KcjORaTkWFs0CY8knt/BGKHfgikX54kJd06H2\n" \
                              "bThqTDHJxc/QIX2+MAi9zFLYbuMZknJZ81mY/zkIh8pTEJxkSBGyF7UOsAMJzGpi\n" \
                              "/xZ17UzZd06F9c6SXX+LqqMdxoY25IGBfe/Vi/1rqOgbPFig7UYccSHqcuNbpjnb\n" \
                              "ZoSgClY9sXvqV9notvsC5tYAAMNElBmKJ1KnAVY9jn7cO25Rhkh4aTkxI+Nop+H3\n" \
                              "ZnYNgX+JzmrlBiVFaYBrA5A2E6HCYRw3DeTWRX/fa7+9KCRzdWWnT/oRB4g2SHK1\n" \
                              "3UHS5uECgYEA9P813g7prATyn5lWLTZsKf4I2f7dzeaUFiShIScMOs3Ie8Nf9RE+\n" \
                              "NILk3zoVo7QGjsHJQ4jJng8x3CY+ZOilxz8DxnqPTnjVab3QuupRxH/5gB4WWs1M\n" \
                              "1w+XMDL/YkP5Gl7nv+oVovubhOSoC3Zrl+JqScAJrlx4Iwe7yrr7sVMCgYEA5S8v\n" \
                              "Skqh4kirJDR8faAeRCfiSTjPQfwkrrd67Wc13GUaYvjuaLSYQryqRWi2VBLtgg3q\n" \
                              "dbV/H7767mjxmHQc6fyhQSIvaZUtH1wNCaF2YTQkY5aePizrihp6bL1+zE+M/i2K\n" \
                              "uf26yzZM/x/xLgV9lPeRssM0HFVC+PlIUm2UL9ECgYEA27+Z8n5Py7J4aPzhVhZt\n" \
                              "eFDaYaYADvIVu7LJL/JVPt1bSN9aT3dS40jQvwTbagVMqHla7A9iQDqCz2MiLeKa\n" \
                              "55c27Kz7ozgwu/K8qRsJkoXzJZ5XEMT6eiAKzPnBJcOwSJ39op71H0YtHUr3C6kB\n" \
                              "Iq+8BECSrmlUfJ8wKswCe+8CgYEAjYq9nxlGr3UH1V1hAUkRSDCFrYkbX3o0wX7p\n" \
                              "uZMDjbEXQcbRnvLiEl1on2NTL8tcyfSEN9w2vcl6OiGwC+KyNMRkZtCRDZkm+/Yi\n" \
                              "yy7f6JqGWscbxfSCJ5HAyEWj+NSslBJWWhXc9kH7/ZihoDWd4JGhdxDdrx3tNczn\n" \
                              "IJkyavECgYEApzdrYvSk+ZwBkfHvo9Z3t7gV9BIsZJG5SZZ/pAgFryBmdVIPTcmV\n" \
                              "mcQLz7DEDP8IKsyGdoqOMwKTvBoxLwvEym1ICFwIzVG2hM8Kzr/Uui6LGKda8TXk\n" \
                              "xcDqZuDxfbwW4EHzIU/ZNb+v5dQYB9GrwmLd/x3i9hZo/lVSd9/aIXc=\n" \
                              "-----END RSA PRIVATE KEY-----\n";


const char* test_client_cert = \
                               "-----BEGIN CERTIFICATE-----\n" \
                               "MIID1jCCAr6gAwIBAgIUE7dBt+qr7xJqsGNjxeQZN0N3EjkwDQYJKoZIhvcNAQEL\n" \
                               "BQAwgYYxCzAJBgNVBAYTAml0MQswCQYDVQQIDAJpdDELMAkGA1UEBwwCaXQxFDAS\n" \
                               "BgNVBAoMC2V4YW1wbGUub3JnMRQwEgYDVQQLDAtleGFtcGxlLm9yZzEUMBIGA1UE\n" \
                               "AwwLZXhhbXBsZS5vcmcxGzAZBgkqhkiG9w0BCQEWDGdhQGdtYWlsLmNvbTAeFw0y\n" \
                               "MjEyMTMxMDU5NTNaFw0yMzEyMTMxMDU5NTNaMHwxCzAJBgNVBAYTAml0MQswCQYD\n" \
                               "VQQIDAJpdDELMAkGA1UEBwwCaXQxITAfBgNVBAoMGEludGVybmV0IFdpZGdpdHMg\n" \
                               "UHR5IEx0ZDETMBEGA1UEAwwKY2xpZW50Lm9yZzEbMBkGCSqGSIb3DQEJARYMZ2FA\n" \
                               "Z21haWwuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA21VzTHsx\n" \
                               "zVYiJyzXp1nUXV9KJPPa0zxLGXi1u7X+JSroHby5gpckZRfcZ8A6QmiU6oSV7Kw/\n" \
                               "LvGR0VjuFrPmyq2UY35c67jcuFgtqr0lcNueFFZE5VsJZYvEBwyQt1PEPcKAOU5v\n" \
                               "v6BgTwAWAoR2fnLen5+MLs+Pg6nh4tb4JBTji+n6vWoKrjjV16AQYtpmmO7yYU6A\n" \
                               "dkOWrUuBcSiIuctnROHa9nFuieY42R05HO1nuPnHSSCtnKf5DFQtS3xIAxvKhUCO\n" \
                               "NxoPZ+dEpCVK3bQhPtShKyvY2cKti6CEFFJKP+/HUfrJWmlm5Cl37MV9f6FcogEp\n" \
                               "PHV31PFvuAYBwwIDAQABo0UwQzAfBgNVHSMEGDAWgBRWc57qbriI1G3uRQ40b091\n" \
                               "3a7rkDAJBgNVHRMEAjAAMBUGA1UdEQQOMAyCCmNsaWVudC5vcmcwDQYJKoZIhvcN\n" \
                               "AQELBQADggEBAF3jyMFSTztahBHPG/+FFYDJHcUVjZn5ZbeykEqzMnNiSaT9Ocf/\n" \
                               "yR+dCOjxV8dB1X9o7BIrjb4RS55S55FxmEL8+jLczKdkLwnWumQKDW6ObFecNJ2k\n" \
                               "q/MP3I6jWOLJmCfIHmNNNsaJYSfAAi1sOPi2F/PuoyI6Yg+oiTbeTINjhWIorUke\n" \
                               "fJMueTDwBb1nXcC5wFI2A943lCKLD0dUCpCBnwb2SciaNKaMHplfUZcDGTxDlZRC\n" \
                               "mYhISMFYFUel5hDP6y3HacfFAhCJpJ0V3D8u4qaGcBZCPmQpcB98hfHHlPL0kLD5\n" \
                               "cNxNs1NZ0k5xhUuS/pCbQ/nSVJauUKB54n4=\n" \
                               "-----END CERTIFICATE-----\n";

=======
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
>>>>>>> 1d215c1232060bbcc3f4b3185e5927c93559ce97
