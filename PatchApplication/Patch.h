#ifndef PATCH_H_
#define PATCH_H_

typedef int BinaryPatchResult;

int SUCCESS            = 0;
int WRONG_TYPE_OF_FILE = 1 << 0;
int ERROR              = 1 << 1;

static BinaryPatchResult binaryPatch(FILE* file_to_crack, const char* file_path) {
    if(strcmp(file_path, "FilesToCrack/CRACK.COM")) {
        return WRONG_TYPE_OF_FILE;
    }

    int offset  = 0x82;
    int command = 0xEB; //jmp
    int value   = 0x33; //jmp value

    fseek(file_to_crack, offset, SEEK_SET);
    fputc(command, file_to_crack);
    fseek(file_to_crack, 1, SEEK_SET);
    fputc(value,   file_to_crack);

    fclose(file_to_crack);

    return SUCCESS;
}

#endif
