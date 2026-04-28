#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/headers/text.h"


void print_dirent(const struct dirent * entry, FILE * log_fp);
void print_stat(const struct stat * st, const char * filename, FILE * log_fp);

int main() 
{        
    FILE * log_fp = fopen("log_words.log", "w+");
    if (log_fp == NULL)
    {
        fprintf(stderr, "Opening log file error\n");
        return 1;
    }

    const char * path = "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/source-text";
    struct dirent * entry = NULL;

    StringArray_t arr = {};
    if (!string_array_init(&arr, MAX_WORDS))
    {
        fclose(log_fp);
        return 1;
    }

    DIR * dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "Opening directory error\n");
        fclose(log_fp);
        string_array_free(&arr);
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        struct stat file_stat = {};
        if (stat(full_path, &file_stat) != 0)
        {
            fprintf(stderr, "stat error\n");
            continue;
        }

        print_dirent(entry, log_fp);
        print_stat(&file_stat, entry->d_name, log_fp);

        if (!collect_words(full_path, &arr))
        {
            fprintf(stderr, "Failed to process file: %s\n", full_path);
        }
    }


    fprintf(log_fp, "Collected words: %zu\n", arr.size);
    strings_array_peep(&arr, 20, log_fp);
    
    if (!export_words(&arr, "../sonnets_data.bin"))
    {
        fprintf(stderr, "error during loading data on disk\n");
    }
    
    string_array_free(&arr);
    closedir(dir);
    fclose(log_fp);

    return 0;
}


void print_dirent(const struct dirent * entry, FILE * log_fp)
{
    assert(entry);

    fprintf(log_fp, "\n");
    fprintf(log_fp, "=== struct dirent ===\n");
    fprintf(log_fp, "d_ini:      %lu (inode number)\n", (unsigned long)entry->d_ino);
    fprintf(log_fp, "d_off:      %ld (offset to next dirent)\n", (long)entry->d_off);
    fprintf(log_fp, "d_reclen:   %d (length of this record)\n", entry->d_reclen);
    fprintf(log_fp, "d_type:     %d (file type) -> ", entry->d_type);

    switch(entry->d_type) 
    {
        case DT_BLK:            fprintf(log_fp, "block device\n"); break;
        case DT_CHR:            fprintf(log_fp, "character device\n"); break;
        case DT_DIR:            fprintf(log_fp, "directory\n"); break;
        case DT_FIFO:           fprintf(log_fp, "FIFO/pipe\n"); break;
        case DT_LNK:            fprintf(log_fp, "symlink\n"); break;
        case DT_REG:            fprintf(log_fp, "regular file\n"); break;
        case DT_SOCK:           fprintf(log_fp, "socket\n"); break;
        case DT_UNKNOWN:        fprintf(log_fp, "unknown\n"); break;
        default:                fprintf(log_fp, "other\n");
    }
    
    fprintf(log_fp, "d_name:    \"%s\" (file name)\n", entry->d_name);
    fprintf(log_fp, "\n");
}


void print_stat(const struct stat * st, const char * filename, FILE * log_fp)
{
    assert(st && filename);

    fprintf(log_fp, "\n");
    fprintf(log_fp, "=== struct stat \"%s\"===\n", filename);
    fprintf(log_fp, "st_size:   %ld bytes (%.2f KB, %.2f MB)\n", 
                                    (long)st->st_size, 
                                    (double)st->st_size / 1024.0, 
                                    (double)st->st_size / (1024.0 * 1024.0));
    fprintf(log_fp, "st_dev:     %lu (device ID)\n", (unsigned long)st->st_dev);
    fprintf(log_fp, "st_ino:     %lu (inode number)\n", (unsigned long)st->st_ino);
    fprintf(log_fp, "\n");
}