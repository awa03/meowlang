# Ideas 

This page serves as a compilation of ideas that may be implemented. 

**Probabilistic execution paths**
```
roll {
    .1 -> run_fun_a(),
    .2 -> run_fun_b(), 
    .6 -> run_fun_c(),

    $ remaining noop $
}
```

**Timed lambda**

These lambda functions will execute in a specified time following call.

```
mut second = ;

lambda:@1000 {
    del second
}
```

**Timed Bound Execution**

Enforce function execution time limits.

```
within:@1000 {
    exp_func(); 
}

$ if exec dnf $
else {
    backup_func();
}

```

**The Last Call -- Revive**

Reexecute the last call, return nil if no call.

```

fun do_something2();

fun do_something(){
    do_something2();
}

do_something();

revive: ($ params $);
```

**Scope Annotations & Key Scopes**

Basically a namespace with keywords for global and local.

```

$ var is 10 in this scope $ 
new var = 10;

<naming> {
    $ var is 20 in this scope $ 
    mut var = 20;
}

fun main() {
    $ prints 10
    meow var;
    
    $ prints 20
    meow <naming>var;
    <naming>var = <naming>var + 1;

    $ prints 21
    meow var; 
}

meow variable;

main();
```


