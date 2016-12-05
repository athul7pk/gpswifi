#ifndef _NVRAM_H_
#define _NVRAM_H_
#define CONFIG_FILE_PATH "/usr/bin/AppData/kios/config"
#define TEMP_FILE_PATH "/tmp/tempconfig"
char *nvram_get(int id, const char *token);
int nvram_bufset(int index, const char *token, const char *value);

#endif
