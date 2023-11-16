#include "monty.h"

global_t vglo;

/**
 * main - the entry point for the Monty interpreter
 * @argc: the number of arguments
 * @argv: the array of arguments
 *
 * Return: Always 0 on success
 */
int main(int argc, char *argv[])
{
    char *file_path;
    size_t len;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    file_path = argv[1];
    len = _strlen(file_path);

    vglo.fd = fopen(file_path, "r");
    if (vglo.fd == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    vglo.buffer = NULL;
    vglo.head = NULL;
    vglo.lifo = 1;
    vglo.cont = 1;

    parse_monty_file();

    fclose(vglo.fd);
    free_vglo();

    return (EXIT_SUCCESS);
}

/**
 * _strlen - to calculate the length of a string
 * @s: the input string
 *
 * Return: Length of the string
 */
size_t _strlen(const char *s)
{
    size_t len = 0;

    while (s[len] != '\0')
        len++;

    return (len);
}

/**
 * parse_monty_file - Parse the Monty file and execute commands
 */
void parse_monty_file(void)
{
    size_t bufsize = 0;
    ssize_t getline_ret;
    char *line = NULL;

    while ((getline_ret = getline(&vglo.buffer, &bufsize, vglo.fd)) != -1)
    {
        vglo.arg = _strtoky(vglo.buffer, " \t\n");

        if (vglo.arg != NULL && vglo.arg[0] != '#')
        {
            get_opcodes(vglo.arg)(&vglo.head, vglo.cont);
        }

        vglo.cont++;
    }

    if (getline_ret == -1 && !feof(vglo.fd))
    {
        fprintf(stderr, "Error: Unable to read line %u\n", vglo.cont);
        exit(EXIT_FAILURE);
    }
}
