#pragma once


/* FUNCTIONS */
int add(unsigned int base, char mask);
int del(unsigned int base, char mask);
char check(unsigned int ip);



/* Internal structures */
typedef struct sIpv4Interval
{
    unsigned int    low;
    unsigned int    high;
    unsigned int    diff;
} sIpv4Interval;

typedef struct sIpv4Prefix
{
    unsigned int    base;
    unsigned int    ext_mask;
    char            short_mask;
} sIpv4Prefix;


