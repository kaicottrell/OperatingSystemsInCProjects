#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RECORDS 100

struct record {
    char fdc_id[20];
    char brand_description[100];
    char expiration_date[20];
    char label_weight[20];
    char location[50];
    char acquisition_date[20];
    char sales_type[20];
    char sample_lot_nbr[20];
    char sell_by_date[20];
    char store_city[50];
    char store_name[50];
    char store_state[15];
    char upc_code[50];
};

int main() {
    char cmd[75];
    char file_name[50];
    FILE *original_file, *copy_file, *final_file;
    int character;

    while (1) {
        // Part 1
        printf("Enter the input file name: \n");
        scanf("%s", file_name);
        sprintf(cmd, "chmod 444 %s", file_name); // can read, can't write or execute

        if (system(cmd) != 0) {
            printf("Error executing command. Try again. \n");
        } else {
            break;
        }
    }
    
    // Part 2
    original_file = fopen(file_name, "r");
    copy_file = fopen("inputTemp.txt", "w");

    // Copy original_file into copy_file
    while ((character = fgetc(original_file)) != EOF) {
        fputc(character, copy_file);
    } 
    fclose(original_file);
    fclose(copy_file);
    printf("File copied over to inputTemp.txt\n");

    // Part 3
    copy_file = fopen("inputTemp.txt", "r");
    struct record records[MAX_RECORDS];
    int record_count = 0;

    // Extract from copy_file and save to each record
    while (fscanf(copy_file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]%*c",
          records[record_count].fdc_id, records[record_count].brand_description,
          records[record_count].expiration_date, records[record_count].label_weight,
          records[record_count].location, records[record_count].acquisition_date,
          records[record_count].sales_type, records[record_count].sample_lot_nbr,
          records[record_count].sell_by_date, records[record_count].store_city,
          records[record_count].store_name, records[record_count].store_state,
          records[record_count].upc_code) == 13) {
        record_count++;
    }

    // Part 4
    fclose(copy_file);

    // 4.1
    printf("%s -> ", records[6].store_name);
    strcpy(records[6].store_name, "\"WILDCATS MARKET\"");
    printf("%s\n", records[6].store_name);

    // 4.2
    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].store_state, "\"CA\"") == 0) {
            strcpy(records[i].store_state, "\"California\"");
        }
    }

    // 4.3
    for (int i = 0; i < record_count; i++) {
        // fdc_id
        memmove(records[i].fdc_id, records[i].fdc_id + 1, strlen(records[i].fdc_id));
        records[i].fdc_id[strlen(records[i].fdc_id) - 1] = '\0';

        // brand_description
        memmove(records[i].brand_description, records[i].brand_description + 1, strlen(records[i].brand_description));
        records[i].brand_description[strlen(records[i].brand_description) - 1] = '\0';

        // expiration_date
        memmove(records[i].expiration_date, records[i].expiration_date + 1, strlen(records[i].expiration_date));
        records[i].expiration_date[strlen(records[i].expiration_date) - 1] = '\0';

        // label_weight
        memmove(records[i].label_weight, records[i].label_weight + 1, strlen(records[i].label_weight));
        records[i].label_weight[strlen(records[i].label_weight) - 1] = '\0';

        // location
        memmove(records[i].location, records[i].location + 1, strlen(records[i].location));
        records[i].location[strlen(records[i].location) - 1] = '\0';

        // acquisition_date
        memmove(records[i].acquisition_date, records[i].acquisition_date + 1, strlen(records[i].acquisition_date));
        records[i].acquisition_date[strlen(records[i].acquisition_date) - 1] = '\0';

        // sales_type
        memmove(records[i].sales_type, records[i].sales_type + 1, strlen(records[i].sales_type));
        records[i].sales_type[strlen(records[i].sales_type) - 1] = '\0';

        // sample_lot_nbr
        memmove(records[i].sample_lot_nbr, records[i].sample_lot_nbr + 1, strlen(records[i].sample_lot_nbr));
        records[i].sample_lot_nbr[strlen(records[i].sample_lot_nbr) - 1] = '\0';

        // sell_by_date
        memmove(records[i].sell_by_date, records[i].sell_by_date + 1, strlen(records[i].sell_by_date));
        records[i].sell_by_date[strlen(records[i].sell_by_date) - 1] = '\0';

        // store_city
        memmove(records[i].store_city, records[i].store_city + 1, strlen(records[i].store_city));
        records[i].store_city[strlen(records[i].store_city) - 1] = '\0';

        // store_name
        memmove(records[i].store_name, records[i].store_name + 1, strlen(records[i].store_name));
        records[i].store_name[strlen(records[i].store_name) - 1] = '\0';

        // store_state
        memmove(records[i].store_state, records[i].store_state + 1, strlen(records[i].store_state));
        records[i].store_state[strlen(records[i].store_state) - 1] = '\0';

        // upc_code
        memmove(records[i].upc_code, records[i].upc_code + 1, strlen(records[i].upc_code));
        records[i].upc_code[strlen(records[i].upc_code) - 1] = '\0';
    }

   //Part 5
    final_file = fopen("final.txt", "w");
    for(int i = 0; i<89; i++){
        fprintf(final_file,"%s  %s  %s  %s\n", records[i].brand_description, records[i].store_city, records[i].store_name, records[i].store_state);
    }
    fclose(final_file);
    return 0;
}
