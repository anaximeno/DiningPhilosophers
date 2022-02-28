# Dining Philosophers and Mutexes
This repo contains a piece of code in C that implements and solves the [Dining Philosophers](https://en.wikipedia.org/wiki/Dining_philosophers_problem) problem using the [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion) property which can be used to control the access to critical sections on concurrent tasks.


### Build
Build the **asynchronous** code using:

```bash
gcc -O3 -finline-functions synchronous.c -pthread -o async
```

Then run with:

```bash
./async
```

---

Build the **synchronous** one using:

```bash
gcc -O3 -finline-functions synchronous.c -pthread -o sync
```

Run with:

```bash
./sync
```