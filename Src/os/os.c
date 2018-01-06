/**
 * OS specific calls for file manipulations. Generic functionality
 * where API between POSIX and Win32 APIs overlap is to be placed here,
 * otherwise the correct support code will be loaded at build time.
 *
 * Authors:
 * Olivier Zardini, Brutal Deluxe Software, Mar. 2012
 * David Stancu, @mach-kernel, January 2018
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>

#if IS_LINUX
#include <strings.h>
#endif

#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <errno.h>

#include "../Dc_Shared.h"
#include "../Dc_Prodos.h"
#include "../Dc_Memory.h"
#include "os.h"

#if !defined(S_ISDIR) && defined(S_IFDIR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#if IS_WINDOWS
#include "win32.c"
#else
#include "posix.c"
#endif

/**
 * Delete file at path
 * @brief os_DeleteFile
 * @param file_path
 */
void os_DeleteFile(char *file_path)
{
  #if IS_WINDOWS
  os_SetFileAttribute(file_path,SET_FILE_VISIBLE);
  #endif
  unlink(file_path);
}

/**
 * Recursively (if necessary) creates a directory. This should work
 * on both POSIX and the classic Win32 C runtime, but will not work
 * with UWP.
 *
 * @brief os_CreateDirectory Create a directory
 * @param directory char *directory
 * @return
 */
int os_CreateDirectory(char *directory)
{
  int error = 0;
  struct stat dirstat;

  char *dir_tokenize = strdup(directory);

  char *buffer = calloc(1, 1024);
  char *token = strtok(dir_tokenize, FOLDER_CHARACTER);

  while (token) {
    if (strlen(buffer) + strlen(token) > 1024) return(-1);
    strcat(buffer, token);

    if (stat(buffer, &dirstat) != 0)
      error = my_mkdir(buffer);
    else if (!S_ISDIR(dirstat.st_mode))
      error = my_mkdir(buffer);

    strcat(buffer, FOLDER_CHARACTER);
    token = strtok(NULL, FOLDER_CHARACTER);
  }

  return error;
}