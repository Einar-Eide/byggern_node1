Dette er koden som kjører på node1 av Byggern' labben.

Debugging: Kjør make debug. Åpne ny terminal, naviger til node1 og kjør avr-gdb -iex "target remote localhost:4242" ./build/a.out