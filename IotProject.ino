#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Cipher.h"

#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 

const char* ssid="iPhone 12 pro";
const char* password="napoli1234";

String card1 = "FFFFFFFF";  // Change this value to the UID of your card.
MFRC522 mfrc522(SS_PIN, RST_PIN);

Cipher* cipher = new Cipher();

const char* hostname = "example.org";
unsigned int port = 11111;
WiFiClientSecure client(hostname);

const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n" \
                           "MIID4jCCAsqgAwIBAgIUbenEW900x1q5XiVRI3szwnzNBSkwDQYJKoZIhvcNAQEL\n" \
                           "BQAwgYYxCzAJBgNVBAYTAml0MQswCQYDVQQIDAJpdDELMAkGA1UEBwwCaXQxFDAS\n" \
                           "BgNVBAoMC2V4YW1wbGUub3JnMRQwEgYDVQQLDAtleGFtcGxlLm9yZzEUMBIGA1UE\n" \
                           "AwwLZXhhbXBsZS5vcmcxGzAZBgkqhkiG9w0BCQEWDGdhQGdtYWlsLmNvbTAeFw0y\n" \
                           "MjEyMDkyMDU0MzlaFw0yMzEyMDkyMDU0MzlaMIGGMQswCQYDVQQGEwJpdDELMAkG\n" \
                           "A1UECAwCaXQxCzAJBgNVBAcMAml0MRQwEgYDVQQKDAtleGFtcGxlLm9yZzEUMBIG\n" \
                           "A1UECwwLZXhhbXBsZS5vcmcxFDASBgNVBAMMC2V4YW1wbGUub3JnMRswGQYJKoZI\n" \
                           "hvcNAQkBFgxnYUBnbWFpbC5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
                           "AoIBAQC8/ZiExCt3m9vcD3iRGINWysb6WuvTHMB+sI59cxBG2jUY2mOUiRNeTOKU\n" \
                           "8IKNna3fqdBADQIzIMsXERKh1i7rGP8MoxOzblB6s3kNw8+7ukUrxi/8RBEjMZDD\n" \
                           "6hS1w4Ar/NUfUz22AFrPEno00NqSLEk9/4jeaEvyFMEO0S1L0TmhmV/E/21APwm8\n" \
                           "KZB0AL7MJLQP7j4iDFg37OoULq7uTexaqRvPofvDa040LTRC9r8NkAJXxhaTPgGv\n" \
                           "glIJW/dkFQ7O67zn2+5kdLb6uqugk1AykZXZWnrjRnLswLl7xwdJp2vU8k1+HtCy\n" \
                           "R/2hFDWVP4VVVi+OsNm1fJr1Yk9nAgMBAAGjRjBEMB8GA1UdIwQYMBaAFFZznupu\n" \
                           "uIjUbe5FDjRvT3XdruuQMAkGA1UdEwQCMAAwFgYDVR0RBA8wDYILZXhhbXBsZS5v\n" \
                           "cmcwDQYJKoZIhvcNAQELBQADggEBADAORO3n5PSTvYN06oe1L2pnXO0XyD1bWyrU\n" \
                           "QEdxlu1CQvj3g7YNztKfwQ8tcCFKK1JkRboZ8GPVJIlw/MSxJha7pLy69Y68Wtgz\n" \
                           "fzJsHYcMPHEtjXnRVsbfYKF6vYBCUUvw6QGGsc07NsNZ//jPt2WUzTY8XvG/CFD7\n" \
                           "rJbPjeridcll5e9mSBiBxhSIs4/4cgwHbhXhHD4pPMuk1XB6Yb1xx7P7T2+2mNg2\n" \
                           "6knD8QoS6u1StfwVgkGNXrocqfam1/dK8UOwiqfQ+6WWA8LEVAr3ABS3zRTJNkXx\n" \
                           "bKSKBJAdqSRJ8aapbpKKL4CjdZu1+pu5uFR+ZYHo+73790OdVJY=\n" \
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

/*
StaticJsonDocument<512>doc;
//wifi>
int address = 0;
EepromStream eepromStream(address, 512);
 void store();
 void read();
void clear();
void getUser();
void createUser(String,String );
void checkRFID();
void deleteUser(String);



 JsonArray users = doc.to<JsonArray>();