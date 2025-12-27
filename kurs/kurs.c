// Необходимые библиотеки
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

// КОНСТАНТЫ И СТРУКТУРЫ

#define MAX_NAME_LEN 25
#define MAX_OS_LEN 20
#define MAX_FAMILY_LEN 20
#define MAX_GPU_MODEL_LEN 30
#define FILENAME_LEN 50
#define MAX_RECORDS 100

// Структура для хранения данных о компьютере
typedef struct {
    char processor[MAX_NAME_LEN];          // Процессор
    int ram_size;                          // Объем оперативной памяти (ГБ)
    char manufacturer[MAX_NAME_LEN];       // Производитель компьютера
    int is_gaming;                         // Игровой компьютер (1-да, 0-нет)
    char cpu_family[MAX_FAMILY_LEN];       // Семейство процессоров
    char gpu_model[MAX_GPU_MODEL_LEN];     // Модель дискретной видеокарты
    char os[MAX_OS_LEN];                   // Операционная система
    int ssd_size;                          // Объем SSD (ГБ)
    int vram_size;                         // Объем видеопамяти (ГБ)
    int cpu_cores;                         // Количество ядер процессора
} Computer;

// ПРОТОТИПЫ ФУНКЦИЙ

// Основные функции программы
void print_welcome_message(void);
void fill_computer_data(Computer* pc);
void print_all_computers(Computer* computers, int count);
void search_by_fields(Computer* computers, int count);
void save_to_file(const char* filename, Computer* computers, int count);
int load_from_file(const char* filename, Computer* computers, int max_count);
void sort_and_print_computers(Computer* computers, int count);
void modify_record_in_file(void);
void add_records_to_file(void);

// Вспомогательные функции
void clear_input_buffer(void);
int get_valid_int(const char* prompt, int min_val);
int get_yes_no(const char* prompt);
void print_computer_table(Computer* computers, int count);

// Функции сравнения для сортировки 
int compare_processor(const void* a, const void* b);
int compare_gaming(const void* a, const void* b);

// MAIN 

int main() {
    setlocale(LC_ALL, "RUS");

    Computer computers[MAX_RECORDS];
    int count = 0;
    int choice;

    print_welcome_message();

    do {
        printf("\n=== ГЛАВНОЕ МЕНЮ ===\n1. Создание новой записи\n2. Поиск записи по заданным полям\n3. Запись и чтение всех данных из файла\n4. Печать всех записей с сортировкой\n5. Изменение отдельных записей из файла\n6. Добавление новых записей в файл\n0. Выход\nВыберите действие (0-6): ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
        case 1: {
            int num_records;
            printf("Сколько записей создать? (макс. %d): ", MAX_RECORDS);
            scanf("%d", &num_records);
            clear_input_buffer();

            if (num_records <= 0 || num_records > MAX_RECORDS) {
                printf("Некорректное количество записей!\n");
                break;
            }

            for (int i = 0; i < num_records; i++) {
                printf("\n=== Запись %d из %d ===\n", i + 1, num_records);
                fill_computer_data(&computers[i]);
            }
            count = num_records;

            printf("\n=== Созданные записи ===\n");
            print_all_computers(computers, count);
            break;
        }

        case 2: {
            search_by_fields(computers, count);
            break;
        }

        case 3: {
            int file_choice;
            printf("\n=== РАБОТА С ФАЙЛАМИ ===\n");
            printf("1. Записать данные в файл\n");
            printf("2. Прочитать данные из файла\n");
            printf("Выберите действие (1-2): ");
            scanf("%d", &file_choice);
            clear_input_buffer();

            if (file_choice == 1) {
                if (count == 0) {
                    printf("Нет данных для сохранения!\n");
                    break;
                }

                char filename[FILENAME_LEN];
                printf("Введите имя файла для сохранения: ");
                fgets(filename, FILENAME_LEN, stdin);
                filename[strcspn(filename, "\n")] = 0;
                save_to_file(filename, computers, count);
            }
            else if (file_choice == 2) {
                char filename[FILENAME_LEN];
                printf("Введите имя файла для загрузки: ");
                fgets(filename, FILENAME_LEN, stdin);
                filename[strcspn(filename, "\n")] = 0;
                count = load_from_file(filename, computers, MAX_RECORDS);
                if (count > 0) {
                    printf("Загружено %d записей\n", count);
                    print_all_computers(computers, count);
                }
            }
            else {
                printf("Неверный выбор!\n");
            }
            break;
        }

        case 4: {
            sort_and_print_computers(computers, count);
            break;
        }

        case 5: {
            modify_record_in_file();
            break;
        }

        case 6: {
            add_records_to_file();
            break;
        }

        case 0:
            printf("Выход из программы...\n");
            break;

        default:
            printf("Неверный выбор! Введите число от 0 до 6.\n");
        }
    } while (choice != 0);

    return 0;
}

// ОПРЕДЕЛЕНИЯ ФУНКЦИЙ 

void print_welcome_message(void) {
    printf("=============================================\n");
    printf("  ПРОГРАММА БАЗЫ ДАННЫХ ПЕРСОНАЛЬНЫХ КОМПЬЮТЕРОВ\n");
    printf("=============================================\n");
    printf("Программа предназначена для работы с записями\n");
    printf("данных о персональных компьютерах:\n");
    printf("- Создание и редактирование записей\n");
    printf("- Поиск по различным полям\n");
    printf("- Сортировка и сохранение в файлы\n");
    printf("=============================================\n\n");
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(const char* prompt, int min_val) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min_val) {
            clear_input_buffer();
            return value;
        }
        printf("Ошибка! Введите целое число >= %d\n", min_val);
        clear_input_buffer();
    }
}

int get_yes_no(const char* prompt) {
    char input;
    while (1) {
        printf("%s (y/n): ", prompt);
        scanf(" %c", &input);
        clear_input_buffer();

        if (input == 'y' || input == 'Y')
            return 1;
        else if (input == 'n' || input == 'N')
            return 0;
        else
            printf("Ошибка! Введите 'y' или 'n'\n");
    }
}

void fill_computer_data(Computer* pc) {
    printf("Введите процессор (например, Intel Core i5): ");
    fgets(pc->processor, MAX_NAME_LEN, stdin);
    pc->processor[strcspn(pc->processor, "\n")] = 0;

    pc->ram_size = get_valid_int("Объем оперативной памяти (ГБ): ", 1);

    printf("Введите производителя компьютера (Dell, HP, Lenovo, Asus и т.д.): ");
    fgets(pc->manufacturer, MAX_NAME_LEN, stdin);
    pc->manufacturer[strcspn(pc->manufacturer, "\n")] = 0;

    pc->is_gaming = get_yes_no("Игровой компьютер?");

    printf("Введите семейство процессоров (например, Core i3, Ryzen 5): ");
    fgets(pc->cpu_family, MAX_FAMILY_LEN, stdin);
    pc->cpu_family[strcspn(pc->cpu_family, "\n")] = 0;

    printf("Введите модель видеокарты (например, NVIDIA GeForce RTX 3060): ");
    fgets(pc->gpu_model, MAX_GPU_MODEL_LEN, stdin);
    pc->gpu_model[strcspn(pc->gpu_model, "\n")] = 0;

    printf("Введите операционную систему (например, Windows 10, Linux): ");
    fgets(pc->os, MAX_OS_LEN, stdin);
    pc->os[strcspn(pc->os, "\n")] = 0;

    pc->ssd_size = get_valid_int("Объем SSD (ГБ, 0 если нет SSD): ", 0);
    pc->vram_size = get_valid_int("Объем видеопамяти (ГБ): ", 0);
    pc->cpu_cores = get_valid_int("Количество ядер процессора: ", 1);
}

void print_computer_table(Computer* computers, int count) {
    if (count == 0) {
        printf("Нет записей для отображения.\n");
        return;
    }

    printf("\n+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
    printf("| Процессор              | ОЗУ  | Производитель   | Игр.| Семейство CPU    | Видеокарта           | ОС               | SSD  | VRAM | Ядра |\n");
    printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");

    for (int i = 0; i < count; i++) {
        printf("| %-22s | %4d | %-15s |  %d  | %-16s | %-20s | %-16s | %4d | %4d | %4d |\n",
            computers[i].processor,
            computers[i].ram_size,
            computers[i].manufacturer,
            computers[i].is_gaming,
            computers[i].cpu_family,
            computers[i].gpu_model,
            computers[i].os,
            computers[i].ssd_size,
            computers[i].vram_size,
            computers[i].cpu_cores);
    }
    printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
}

void print_all_computers(Computer* computers, int count) {
    print_computer_table(computers, count);
}

void search_by_fields(Computer* computers, int count) {
    if (count == 0) {
        printf("Нет данных для поиска! Сначала создайте записи.\n");
        return;
    }

    int search_choice;
    printf("\n=== ПОИСК ЗАПИСЕЙ ===\n");
    printf("1. По процессору\n");
    printf("2. По игровому статусу (1-да, 0-нет)\n");
    printf("3. По обоим полям одновременно\n");
    printf("Выберите тип поиска (1-3): ");
    scanf("%d", &search_choice);
    clear_input_buffer();

    if (search_choice == 1) {
        char processor[MAX_NAME_LEN];
        printf("Введите процессор для поиска: ");
        fgets(processor, MAX_NAME_LEN, stdin);
        processor[strcspn(processor, "\n")] = 0;

        int found = 0;
        printf("\n=== РЕЗУЛЬТАТЫ ПОИСКА ПО ПРОЦЕССОРУ '%s' ===\n", processor);
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
        printf("| Процессор              | ОЗУ  | Производитель   | Игр.| Семейство CPU    | Видеокарта           | ОС               | SSD  | VRAM | Ядра |\n");
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(computers[i].processor, processor) == 0) {
                printf("| %-22s | %4d | %-15s |  %d  | %-16s | %-20s | %-16s | %4d | %4d | %4d |\n",
                    computers[i].processor,
                    computers[i].ram_size,
                    computers[i].manufacturer,
                    computers[i].is_gaming,
                    computers[i].cpu_family,
                    computers[i].gpu_model,
                    computers[i].os,
                    computers[i].ssd_size,
                    computers[i].vram_size,
                    computers[i].cpu_cores);
                found = 1;
            }
        }

        if (found) {
            printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
            printf("Найдено записей: %d\n", found);
        }
        else {
            printf("|                                      ЗАПИСИ НЕ НАЙДЕНЫ                                                        |\n");
            printf("+---------------------------------------------------------------------------------------------------------------+\n");
        }
    }
    else if (search_choice == 2) {
        int gaming;
        printf("Игровой компьютер? (1-да, 0-нет): ");
        scanf("%d", &gaming);
        clear_input_buffer();

        const char* status = gaming ? "игровые" : "не игровые";
        int found = 0;
        printf("\n=== РЕЗУЛЬТАТЫ ПОИСКА %s КОМПЬЮТЕРОВ ===\n", status);
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
        printf("| Процессор              | ОЗУ  | Производитель   | Игр.| Семейство CPU    | Видеокарта           | ОС               | SSD  | VRAM | Ядра |\n");
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");

        for (int i = 0; i < count; i++) {
            if (computers[i].is_gaming == gaming) {
                printf("| %-22s | %4d | %-15s |  %d  | %-16s | %-20s | %-16s | %4d | %4d | %4d |\n",
                    computers[i].processor,
                    computers[i].ram_size,
                    computers[i].manufacturer,
                    computers[i].is_gaming,
                    computers[i].cpu_family,
                    computers[i].gpu_model,
                    computers[i].os,
                    computers[i].ssd_size,
                    computers[i].vram_size,
                    computers[i].cpu_cores);
                found = 1;
            }
        }

        if (found) {
            printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
            printf("Найдено записей: %d\n", found);
        }
        else {
            printf("|                                      ЗАПИСИ НЕ НАЙДЕНЫ                                                        |\n");
            printf("+---------------------------------------------------------------------------------------------------------------+\n");
        }
    }
    else if (search_choice == 3) {
        char processor[MAX_NAME_LEN];
        int gaming;
        int use_processor = 0, use_gaming = 0;

        printf("Искать по процессору? (1-да, 0-нет): ");
        scanf("%d", &use_processor);
        clear_input_buffer();

        if (use_processor) {
            printf("Введите процессор для поиска: ");
            fgets(processor, MAX_NAME_LEN, stdin);
            processor[strcspn(processor, "\n")] = 0;
        }

        printf("Искать по игровому статусу? (1-да, 0-нет): ");
        scanf("%d", &use_gaming);
        clear_input_buffer();

        if (use_gaming) {
            printf("Игровой компьютер? (1-да, 0-нет): ");
            scanf("%d", &gaming);
            clear_input_buffer();
        }

        if (!use_processor && !use_gaming) {
            printf("Не выбраны поля для поиска!\n");
            return;
        }

        printf("\n=== РЕЗУЛЬТАТЫ ПОИСКА ===\n");
        if (use_processor && use_gaming) {
            printf("Критерии: процессор='%s' и игровой статус=%d\n", processor, gaming);
        }
        else if (use_processor) {
            printf("Критерий: процессор='%s'\n", processor);
        }
        else if (use_gaming) {
            printf("Критерий: игровой статус=%d\n", gaming);
        }

        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
        printf("| Процессор              | ОЗУ  | Производитель   | Игр.| Семейство CPU    | Видеокарта           | ОС               | SSD  | VRAM | Ядра |\n");
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");

        int found = 0;
        for (int i = 0; i < count; i++) {
            int match = 1;

            if (use_processor && strcmp(computers[i].processor, processor) != 0)
                match = 0;
            if (use_gaming && computers[i].is_gaming != gaming)
                match = 0;

            if (match) {
                printf("| %-22s | %4d | %-15s |  %d  | %-16s | %-20s | %-16s | %4d | %4d | %4d |\n",
                    computers[i].processor,
                    computers[i].ram_size,
                    computers[i].manufacturer,
                    computers[i].is_gaming,
                    computers[i].cpu_family,
                    computers[i].gpu_model,
                    computers[i].os,
                    computers[i].ssd_size,
                    computers[i].vram_size,
                    computers[i].cpu_cores);
                found = 1;
            }
        }

        if (found) {
            printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
            printf("Найдено записей: %d\n", found);
        }
        else {
            printf("|                                      ЗАПИСИ НЕ НАЙДЕНЫ                                                        |\n");
            printf("+---------------------------------------------------------------------------------------------------------------+\n");
        }
    }
    else {
        printf("Неверный выбор!\n");
    }
}

void save_to_file(const char* filename, Computer* computers, int count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка открытия файла '%s' для записи!\n", filename);
        return;
    }

    fprintf(file, "Процессор,ОЗУ(ГБ),Производитель,Игровой,Семейство_CPU,Видеокарта,ОС,SSD(ГБ),VRAM(ГБ),Ядра\n");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d,%s,%d,%s,%s,%s,%d,%d,%d\n",
            computers[i].processor,
            computers[i].ram_size,
            computers[i].manufacturer,
            computers[i].is_gaming,
            computers[i].cpu_family,
            computers[i].gpu_model,
            computers[i].os,
            computers[i].ssd_size,
            computers[i].vram_size,
            computers[i].cpu_cores);
    }

    fclose(file);
    printf("Данные успешно сохранены в файл '%s' (%d записей)\n", filename, count);
}

int load_from_file(const char* filename, Computer* computers, int max_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла '%s' для чтения!\n", filename);
        return 0;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    int loaded = 0;
    while (loaded < max_count && fgets(buffer, sizeof(buffer), file)) {
        char* token = strtok(buffer, ",");
        if (!token) continue;

        strncpy(computers[loaded].processor, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, ",");
        if (token) computers[loaded].ram_size = atoi(token);

        token = strtok(NULL, ",");
        if (token) strncpy(computers[loaded].manufacturer, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, ",");
        if (token) computers[loaded].is_gaming = atoi(token);

        token = strtok(NULL, ",");
        if (token) strncpy(computers[loaded].cpu_family, token, MAX_FAMILY_LEN - 1);

        token = strtok(NULL, ",");
        if (token) strncpy(computers[loaded].gpu_model, token, MAX_GPU_MODEL_LEN - 1);

        token = strtok(NULL, ",");
        if (token) strncpy(computers[loaded].os, token, MAX_OS_LEN - 1);

        token = strtok(NULL, ",");
        if (token) computers[loaded].ssd_size = atoi(token);

        token = strtok(NULL, ",");
        if (token) computers[loaded].vram_size = atoi(token);

        token = strtok(NULL, ",\n");
        if (token) computers[loaded].cpu_cores = atoi(token);

        loaded++;
    }

    fclose(file);
    return loaded;
}


int compare_processor(const void* a, const void* b) {
    return strcmp(((Computer*)a)->processor, ((Computer*)b)->processor);
}

int compare_gaming(const void* a, const void* b) {
    return ((Computer*)a)->is_gaming - ((Computer*)b)->is_gaming;
}


void sort_and_print_computers(Computer* computers, int count) {
    if (count == 0) {
        printf("Нет данных для отображения! Сначала создайте записи.\n");
        return;
    }

    int sort_field;
    printf("\n=== СОРТИРОВКА И ВЫВОД ЗАПИСЕЙ ===\n");
    printf("1. По процессору (алфавитный порядок)\n");
    printf("2. По игровому статусу (сначала неигровые, потом игровые)\n");
    printf("Выберите поле для сортировки (1-2): ");
    scanf("%d", &sort_field);
    clear_input_buffer();

    if (sort_field == 1) {
        qsort(computers, count, sizeof(Computer), compare_processor);
        printf("\n=== Отсортированные записи (по процессору) ===\n");
        print_all_computers(computers, count);
    }
    else if (sort_field == 2) {
        qsort(computers, count, sizeof(Computer), compare_gaming);
        printf("\n=== Отсортированные записи (по игровому статусу) ===\n");
        print_all_computers(computers, count);
    }
    else {
        printf("Неверный выбор! Доступны только варианты 1 или 2.\n");
    }
}

void modify_record_in_file(void) {
    char filename[FILENAME_LEN];
    printf("Введите имя файла: ");
    fgets(filename, FILENAME_LEN, stdin);
    filename[strcspn(filename, "\n")] = 0;

    Computer temp_computers[MAX_RECORDS];
    int temp_count = load_from_file(filename, temp_computers, MAX_RECORDS);

    if (temp_count == 0) {
        printf("Файл пуст или не существует!\n");
        return;
    }

    printf("\n=== Содержимое файла ===\n");
    print_all_computers(temp_computers, temp_count);

    int index;
    printf("Введите индекс записи для изменения (0-%d): ", temp_count - 1);
    scanf("%d", &index);
    clear_input_buffer();

    if (index >= 0 && index < temp_count) {
        printf("\n=== ИЗМЕНЕНИЕ ЗАПИСИ %d ===\n", index);
        printf("Текущие данные:\n");

        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
        printf("| Процессор              | ОЗУ  | Производитель   | Игр.| Семейство CPU    | Видеокарта           | ОС               | SSD  | VRAM | Ядра |\n");
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");
        printf("| %-22s | %4d | %-15s |  %d  | %-16s | %-20s | %-16s | %4d | %4d | %4d |\n",
            temp_computers[index].processor,
            temp_computers[index].ram_size,
            temp_computers[index].manufacturer,
            temp_computers[index].is_gaming,
            temp_computers[index].cpu_family,
            temp_computers[index].gpu_model,
            temp_computers[index].os,
            temp_computers[index].ssd_size,
            temp_computers[index].vram_size,
            temp_computers[index].cpu_cores);
        printf("+------------------------+------+-----------------+-----+------------------+----------------------+------------------+------+------+------+\n");

        printf("\nВведите новые данные:\n");
        fill_computer_data(&temp_computers[index]);

        save_to_file(filename, temp_computers, temp_count);
        printf("Запись успешно изменена и сохранена в файл!\n");
    }
    else {
        printf("Неверный индекс!\n");
    }
}

void add_records_to_file(void) {
    char filename[FILENAME_LEN];
    printf("Введите имя файла: ");
    fgets(filename, FILENAME_LEN, stdin);
    filename[strcspn(filename, "\n")] = 0;

    Computer new_computers[MAX_RECORDS];
    int new_count;

    printf("Сколько новых записей добавить? ");
    scanf("%d", &new_count);
    clear_input_buffer();

    if (new_count <= 0) {
        printf("Некорректное количество записей!\n");
        return;
    }

    for (int i = 0; i < new_count; i++) {
        printf("\n=== Новая запись %d из %d ===\n", i + 1, new_count);
        fill_computer_data(&new_computers[i]);
    }

    Computer existing_computers[MAX_RECORDS];
    int existing_count = load_from_file(filename, existing_computers, MAX_RECORDS);

    int total_count = existing_count;
    for (int i = 0; i < new_count && total_count < MAX_RECORDS; i++) {
        existing_computers[total_count] = new_computers[i];
        total_count++;
    }

    save_to_file(filename, existing_computers, total_count);

    printf("Успешно добавлено %d новых записей в файл '%s'\n", new_count, filename);
}