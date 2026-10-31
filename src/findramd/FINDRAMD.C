/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Panthera Softwares
 */
#include <dos.h>
#include <string.h>

int main(void)
{
    struct find_t result;
    char path[] = "C:\\*.*";
    char drive;

    for (drive = 'C'; drive <= 'Z'; ++drive) {
        path[0] = drive;
        if (_dos_findfirst(path, _A_VOLID, &result) == 0) {
            if ((result.attrib & _A_VOLID) != 0 &&
                stricmp(result.name, "BITDISK") == 0) {
                _dos_findclose(&result);
                return (drive - 'A') + 1;
            }
            _dos_findclose(&result);
        }
    }
    return 255;
}
