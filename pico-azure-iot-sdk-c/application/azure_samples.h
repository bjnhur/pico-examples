
#ifndef PICO_AZURE_SAMPLES_H
#define PICO_AZURE_SAMPLES_H

#ifdef __cplusplus
extern "C"
{
#endif

    extern const char pico_az_connectionString[];
    extern const char pico_az_x509connectionString[];
    extern const char pico_az_x509certificate[];
    extern const char pico_az_x509privatekey[];
    extern const char pico_az_COMMON_NAME[];
    extern const char pico_az_CERTIFICATE[];
    extern const char pico_az_PRIVATE_KEY[];
    extern const char pico_az_id_scope[];

    void iothub_ll_telemetry_sample(void);
    void iothub_ll_c2d_sample(void);
    void iothub_ll_client_x509_sample(void);
    void prov_dev_client_ll_sample(void);

#ifdef __cplusplus
}
#endif

#endif /* PICO_AZURE_SAMPLES_H */
