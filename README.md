# Debug Plugin

---
## Base Logs
* ``DT_DISPLAY(FORMAT, ...)``
* ``DT_ERROR(FORMAT, ...)``
* ``DT_BRAKEPOINT()``
### Example:
``DT_DISPLAY("{0} {1}", "Hello", "World");``
### Pros:
* Convenient syntax
* Also log in UE log output
### Cons:
* You MUST pass at least one argument. ``DT_DISPLAY("Test")`` will crush
* You MUST pass number into brackets. ``DT_DISPLAY("{}", 1)`` will crush
## Chained logs
* ``DT_CHAINED_DISPLAY() << A << B``
* ``DT_CHAINED_ERROR() << A << B``
### Example:
``DT_CHAINED_ERROR() << "Hello " << "World";``
### Pros:
* Similar with std::cout syntax
* Cant crush(I think)
* Can print TArray of int, float and Objects that derives from UObject
### Cons:
* May have poorer performance
* Bad readebility
* Can accept only float, int32, const [w]char*, TArray<int|float|derived from UObject> 
## Return
* ``DT_RETURN(CONDITION)`` If CONDITION == false, than ``return`` from function and log error
* ``DT_RETURN_A(CONDITION, TO_RET)`` If CONDITION == false, than ``return TO_RET`` from function and log error
## NO_LOGGER
* ``*_NO_LOGGER`` log only into UE logs skipping DT logs. Useful when DT logs are unavailable

---
## You can see logs in 3 sources:
* In Game, in Widget(But you should add it in first)
* * Control: num 0(Go to logger and back), num 5,2(up and down)
* In Editor, in Window(But u should manually refresh it)
* In File(U should enable this feature in settings)
