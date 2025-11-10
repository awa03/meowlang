# Lifetime

Lifetime/Scoping is `meow`'s biggest selling point!
Get granular control over variable lifetime and scoping.

## Lifetime

Borrowing from rust you can manually define the lifetime of a variable in a few ways.

**Brace Lifetime**


```meow
{
new variable = 5;
}
$ variable scope removed
```

We can also use the `new` and `del` keywords to modify and delete variables from a specified scope.

> [!NOTE]
> Variables may only exist within a singular scope path at a time.

```
new variable = 5;

if(variable == 5){
    new variable;
} $ if scope terminates

$ variable no longer exists
new variable = 10; $ redeclaring
del variable;      $ variable no longer exists
```



