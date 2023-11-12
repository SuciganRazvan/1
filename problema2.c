#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>

int copyFile(const char *source, const char *destination) {
    FILE *sourceFile, *destFile;
    char buffer[1024];
    int bytesRead;

    sourceFile = fopen(source, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return 0;
    }

    destFile = fopen(destination, "wb");
    if (destFile == NULL) {
        perror("Error creating destination file");
        fclose(sourceFile);
        return 0;
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);

    return 1;
}

void copyFilesRecursive(const char *sourceDir, const char *destDir, const char *extension, int minSize) {
    struct _finddata_t fileInfo;
    intptr_t handle;
    char sourcePath[_MAX_PATH], destPath[_MAX_PATH];

    strcpy(sourcePath, sourceDir);
    strcat(sourcePath, "\\*.*");

    handle = _findfirst(sourcePath, &fileInfo);

    if (handle == -1) {
        return;
    }

    do {
        if (strcmp(fileInfo.name, "..") == 0 || strcmp(fileInfo.name, ".") == 0) {
            continue;
        }

        strcpy(sourcePath, sourceDir);
        strcat(sourcePath, "\\");
        strcat(sourcePath, fileInfo.name);

        strcpy(destPath, destDir);
        strcat(destPath, "\\");
        strcat(destPath, fileInfo.name);

        if (fileInfo.attrib & _A_SUBDIR) {
            // Recursively copy subdirectories
            mkdir(destPath);
            copyFilesRecursive(sourcePath, destPath, extension, minSize);
        } else {
            if ((extension == NULL || (strrchr(fileInfo.name, '.') && strstr(strrchr(fileInfo.name, '.'), extension) != NULL)) &&
        (minSize == 0 || fileInfo.size >= minSize)) {
        if (copyFile(sourcePath, destPath)) {
            printf("Copied: %s -> %s\n", sourcePath, destPath);
        }
                }
            }
        
    } while (_findnext(handle, &fileInfo) == 0);

    _findclose(handle);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: cpd [fanion...] director_sursa director_destinatie\n");
        return 1;
    }

    char *sourceDir = argv[2];
    char *destDir = argv[3];

    if (strcmp(sourceDir, destDir) == 0) {
        printf("Error: Source and destination directories are the same.\n");
        return 1;
    }

    char *extension = NULL;
    int minSize = 0;

   

for (int i = 1; i < argc - 1; i++) {
    if (strncmp(argv[i], "eext.", 5) == 0) {
        extension = argv[i] + 5;
    }
    if (strcmp(argv[i], "eext") == 0) {
        extension = ".ext";
        minSize=0;
    }
      if (argv[i][0] == 'l') {
        minSize = atoi(argv[i] + 1);
    }
}

    copyFilesRecursive(sourceDir, destDir, extension, minSize);
    return 0;
}