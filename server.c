/*
                PHATCRAFTWIKI
    PhatcraftWiki là một server tìm kiếm và quản lý dữ liệu theo kiểu "Wiki-like"
    Dự án được xây dựng và phát triển bởi Phatcraft
    Thông tin dự án: https://github.com/Phatcraft/phatcraftwiki  
*/


/*
    CÁC CẤU HÌNH TRÊN SERVER
    Tại đây triển khai các cấu hình cứng trên server
*/
#define KILOBYTE 1024
#define BUFFER_SIZE_KB 4
#define MAX_CLIENTS 5


/*
    THƯ VIỆN DỰ ÁN
    Tại đây triển khai các thư viện được sử dụng trong dự án
*/
// Các thư viện chung
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
    FILE CONFIG
    Tại đây triển kai cấu trúc config và hệ thống xử lý file config
*/
// Cấu trúc config
typedef struct {
    int http_port, https_port;
    char domain[KILOBYTE], certificate[KILOBYTE], private_key[KILOBYTE];
}Config;
// Xử lý file config
Config read_config(char *filename){
    // Tạo config rỗng
    Config config;

    // Mở file config
    FILE *config_file = fopen(filename, "r");
    if(config_file == NULL){
        printf("Lỗi khi load config: Không tìm thấy file\n");
        return config;
    }

    // Đọc file config
    char config_line[KILOBYTE * 2];
    while(fgets(config_line, sizeof config_line, config_file) != NULL){
        // Kiểm tra có phải là line rỗng hay comment hay không
        if(config_line[0] == '#' || config_line[0] == '\n') continue;

        // Lấy key và value từ line
        char key[KILOBYTE], value[KILOBYTE];
        sscanf(config_line, "%s = %s\n", key, value);

        // Lọc key
        if(strcmp(key, "HTTP_PORT") == 0) config.http_port = atoi(value);
        if(strcmp(key, "HTTPS_PORT") == 0) config.https_port = atoi(value);

        if(strcmp(key, "DOMAIN") == 0) snprintf(config.domain, KILOBYTE, "%s", value);
        if(strcmp(key, "CERTIFICATE") == 0) snprintf(config.certificate, KILOBYTE, "%s", value);
        if(strcmp(key, "PRIVATE_KEY") == 0) snprintf(config.private_key, KILOBYTE, "%s", value);
    }

    return config;
}


/*
    SERVER CHÍNH
    Tại đây triển khai load config của server và các dịch vụ dưới dạng các thread
*/
int main(int argc, char *argv[]){
    // Kiểm tra đã nạp đủ thông số chưa -> nếu chưa, yêu cầu nhập lại
    if(argc < 2){
        printf("Không có file config.\n");
        printf("Bạn cần nhập đúng theo cấu trúc: %s <tên file config>\n", argv[0]);
        return 0;
    }
    // Lưu tên file config & load config
    char *config_filename = argv[1];
    Config config = read_config(config_filename);

    printf("Domain: %s\n", config.domain);
    printf("Ports: [%d/%d]\n", config.http_port, config.https_port);
    printf("Certificate: %s\nPrivate key: %s\n", config.certificate, config.private_key);

    return 0;
}
