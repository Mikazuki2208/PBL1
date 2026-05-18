# PBL1
Build a student list management application
file chính

file tải về
  code_old.c
  name_class.txt
  class

terminal
  "gcc code_old.c -o main"
  "./main"

file tải về
  main.c
  manager.c
  manager.h
  name_class.txt
  class

terminal
  "nix-shell -p gcc ncurses" (dùng lần đầu để tạo môi trường tạm thời)
  "gcc main.c manager.c -o main -lncursesw"
  "./main"
