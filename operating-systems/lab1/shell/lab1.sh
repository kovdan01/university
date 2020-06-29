#!/bin/sh

interrupt_count=0

interrupt_handler()
{
    interrupt_detected=1
    interrupt_count=`expr $interrupt_count + 1`
    if [ $interrupt_count -gt 5 ]
      then
        echo List of *.c files in this catalogue:
        ls -a *.c
      fi
}

exit_program()
{
    echo ""
    exit 1
}

read_complete=0
compile_complete=0
run_complete=0
trap "interrupt_handler" SIGINT
trap "exit_program" SIGTSTP

while :
  do
    interrupt_detected=0
    error=0

    if [ $read_complete -eq 0 ]
      then
        echo Enter file name:
        read c_filename 2> /dev/null
        if [ $interrupt_detected -eq 1 ]
          then
            continue
          fi
      fi
    read_complete=1

    if [ $compile_complete -eq 0 ]
      then
        echo [INFO] Compiling "$c_filename"...
        g++ "$c_filename" -o "$c_filename.out"
        compile_result=$?
        if [ $interrupt_detected -eq 0 ] && [ $compile_result -ne 0 ]
          then
            echo [INFO] Error while compiling "$c_filename"
            error=1
          fi
        if [ $interrupt_detected -eq 1 ]
          then
            continue
          fi
        if [ $error -eq 1 ]
          then
            read_complete=0
            compile_complete=0
            run_complete=0
            continue
          fi
      fi
    compile_complete=1

    if [ $run_complete -eq 0 ]
      then
        echo [INFO] Running...
        ./"$c_filename.out"
        if [ $interrupt_detected -eq 1 ]
          then
            continue
          fi
      fi
    run_complete=1

    read_complete=0
    compile_complete=0
    run_complete=0
  done
