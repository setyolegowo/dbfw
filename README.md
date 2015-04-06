Database Firewall
====

Setyo Legowo / 13511071

Aplikasi untuk Tugas Akhir Sarjana Teknik Informatika ITB

# How to compile

## Package

In order to compile this application you need the following packages:

1. g++
2. make
3. libpcre
4. libev
5. flex
6. bison

For Ubuntu and Debian I had to install:

1. libev-dev
2. libpcre3-dev
3. libpcre3
4. flex
5. bison
6. make
7. g++
8. gcc

## Compile

Just execute `make` within this directory

# How to run

Before run the database firewall, you should create blank file `/var/log/dbfw.log`. That URI is written in configuration file `dbfw.conf`. And then you run with `./dbfw -p <configuration folder>`.
