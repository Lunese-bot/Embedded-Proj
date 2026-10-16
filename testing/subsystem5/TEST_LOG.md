# Subsystem 5 — test log

**Subsystem:** Wi-Fi/MQTT application interface (Buddy 1)  ·  **Student ID:** <student ID>  ·  **Code:** `app_program/subsystem5/`

See `testing/README.md` for how to run tests and record results.

## Test cases

Define a test here before running it. Test IDs never change.

| ID | What is tested | Requirement / interface | Method | Steps / how | Expected result |
|---|---|---|---|---|---|
| S5-U01 | communication interfaces run in task context | | auto | `subsystem5_test.c` | `[TEST] S5-U01 PASS` |
| S5-U02 | valid `start` command reaches the command queue | | auto | `subsystem5_test.c` | `[TEST] S5-U02 PASS` |
| S5-U03 | unknown command payload is rejected | | auto | `subsystem5_test.c` | `[TEST] S5-U03 PASS` |
| S5-U04 | telemetry snapshot passes through the bounded queue | | auto | `subsystem5_test.c` | `[TEST] S5-U04 PASS` |

*Method:* **auto** = `TEST_CHECK` in code, result printed on the console;
**manual** = a person observes the result (LED, scope, meter, phone app…).

## Results

Add a new row for every run. Never edit or delete old rows.

| Date | Test ID | Commit | Tester | Result | Log / evidence | Notes |
|---|---|---|---|---|---|---|
| YYYY-MM-DD | S5-U01 | `abc1234` | | PASS / FAIL | `logs/YYYY-MM-DD_….txt` | |
