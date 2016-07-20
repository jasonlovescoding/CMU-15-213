#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#define MAXLEN 100
int s,S,E,b;
int hits=0, misses=0, evictions=0;

typedef struct _Line
{
    int v;
    int tag;
    int LRU_p; // priority for LRU logic
} Line;

typedef enum _Exec_Rst
{HIT, MISS, MISS_HIT, MISS_EVICTION, MISS_EVICTION_HIT} Exec_Rst;

long long int get_Set_Index(long long int address)
{
    long long int s_mask = (1<<s)-1;
    return (address>>b)&s_mask;
}

long long int get_Tag(long long int address)
{
    long long int tag_mask = (1<<(64-s-b))-1; // 64-bit system
    return (address>>(s+b))&tag_mask;
}

void update_LRU(long long int set_index, int id,
                long long int tag, Line** cache)
{
    cache[set_index][id].v=1;
    cache[set_index][id].tag=tag;
    int i;
    for(i=0;i<E;i++)
    {
        if(cache[set_index][i].v==1
           &&cache[set_index][i].LRU_p>cache[set_index][id].LRU_p)
        {
            cache[set_index][i].LRU_p--;
        }
    }
    cache[set_index][id].LRU_p=E-1;
}

Exec_Rst exec_Instr(char instr[], Line** cache)
{
    long long int address;
    int size;
    char type;
    sscanf(instr, " %c %llx,%d", &type, &address, &size);
    long long int set_index=get_Set_Index(address);
    long long int tag=get_Tag(address);
    int i;
    for(i=0;i<E;i++)
    {
        if(cache[set_index][i].v==1
           && cache[set_index][i].tag==tag)
        {// find a hit
            hits++;
            update_LRU(set_index,i,tag,cache);
            if(type=='M')
            {// Modify hits twice
                hits++;
            }
            return HIT;
        }
    }
    misses++; // not a hit
    for(i=0;i<E;i++)
    {
        if(cache[set_index][i].v==0)
        {// find vacancy, no eviction needed
            update_LRU(set_index,i,tag,cache);
            if(type=='M')
            {// Modify hits after misses
                hits++;
                return MISS_HIT;
            }
            return MISS;
        }
    }
    evictions++; // no vacancy, meaning there should be an eviction
    for(i=0;i<E;i++)
    {
        if(cache[set_index][i].LRU_p==0)
        {
            update_LRU(set_index,i,tag,cache);
            if(type=='M')
            {
                hits++;
                return MISS_EVICTION_HIT;
            }
            return MISS_EVICTION;
        }
    }
    return -1;
}

Line** init_Cache()
{
    Line **cache=(Line **)malloc(S*sizeof(Line *));
    int i,j;
    for(i=0;i<S;i++)
    {
        cache[i]=(Line *)malloc(E*sizeof(Line));
        for(j=0;j<E;j++)
        {
            cache[i][j].v=0; // set all lines unwarmed
            cache[i][j].tag=-1;
            cache[i][j].LRU_p=-1;
        }
    }
    return cache;
}

int main(int argc, char *argv[])
{
    int verbose=0;
    char t[MAXLEN];
    int opt;
    while((opt=getopt(argc,argv,"hvs:E:b:t:"))!=-1)
    {// see "man 3 getopt" for help
        switch (opt)
        {
        case 'v':
            verbose=1;
            break;
        case 's':
            s=atoi(optarg);
            S=1<<s;
            break;
        case 'E':
            E=atoi(optarg);
            break;
        case 'b':
            b=atoi(optarg);
            break;
        case 't':
            strcpy(t,optarg);
            break;
        default:
            fprintf(stderr,"Usage: %s \n",argv[0]);
            exit(1);
        }
    }
    Line **cache=init_Cache();
    FILE* fin;
    char instr[MAXLEN];
    int exec_rst;
    if((fin=fopen(t,"r"))==NULL)
    {
        printf("Error: File not found.\n");
        exit(1);
    }
    while(fgets(instr,MAXLEN,fin))
    {
        if(instr[0]==' ')
        {// not an instruction load
            exec_rst=exec_Instr(instr, cache);
            if(verbose)
            {
                switch (exec_rst)
                {
                case HIT:
                    printf("%s hit\n",instr+1);
                    break;
                case MISS:
                    printf("%s miss\n",instr+1);
                    break;
                case MISS_HIT:
                    printf("%s miss hit\n",instr+1);
                    break;
                case MISS_EVICTION:
                    printf("%s miss eviction\n",instr+1);
                    break;
                case MISS_EVICTION_HIT:
                    printf("%s miss eviction hit\n",instr+1);
                    break;
                default:
                    printf("Error.\n");
                }
            }
        }
    }

    printSummary(hits,misses,evictions);
    return 0;
}
