#ifndef PASSWORD_MANAGER_C_API_H
#define PASSWORD_MANAGER_C_API_H

#include <stddef.h>

#if defined(_WIN32)
#define PM_API __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define PM_API __attribute__((visibility("default")))
#else
#define PM_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

PM_API int pm_create_key_file(const char* password);
PM_API int pm_key_file_exists(void);
PM_API int pm_validate_password(const char* password);
PM_API int pm_load_data(void);
PM_API int pm_save_data(void);
PM_API int pm_add_email(const char* email_address);
PM_API int pm_remove_email(const char* email_address);
PM_API int pm_get_email_count(void);
PM_API char* pm_get_email_address(int index);
PM_API int pm_add_account(const char* email_address, const char* company,
						  const char* password, const char* name);
PM_API int pm_remove_account(const char* email_address, int account_id);
PM_API int pm_get_account_count(const char* email_address);
PM_API int pm_get_account_id(const char* email_address, int index);
PM_API char* pm_get_account_company(const char* email_address, int account_id);
PM_API char* pm_get_account_password(const char* email_address, int account_id);
PM_API char* pm_get_account_name(const char* email_address, int account_id);
PM_API int pm_set_account_id(const char* email_address, int account_id, int value);
PM_API int pm_set_account_company(const char* email_address, int account_id,
								  const char* value);
PM_API int pm_set_account_password(const char* email_address, int account_id,
								   const char* value);
PM_API int pm_set_account_name(const char* email_address, int account_id,
							   const char* value);
PM_API char* pm_encrypt_string(const char* plaintext);
PM_API char* pm_decrypt_string(const char* ciphertext);
PM_API void pm_string_free(char* value);

#ifdef __cplusplus
}
#endif

#endif
