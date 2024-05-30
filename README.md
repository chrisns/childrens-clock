# Children's IoT Clock


[![Made for ESPHome](https://esphome.io/_static/made-for-esphome-black-on-white.svg)](https://esphome.io/guides/made_for_esphome)

## Development

This is all intended to use [dev containers](https://containers.dev/)

### Run tests

```bash
rake test
```

```raw
Test 'TestMain.c'
-----------------
Running TestMain.out...

--------------------
OVERALL TEST SUMMARY
--------------------
TESTED:  5
PASSED:  5
FAILED:  0
IGNORED: 0
```

### Run tests on code change

```bash
guard
```

### Build
```bash
rake build
```