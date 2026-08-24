## API-method `rate`

### Signature
``` c++
bool rate(int Limit, int DurationSec);
```

### Description
Set download speed limit.

### Arguments
- **Limit** `(int)` - new download speed limit in KBytes/second. Value `0` disables speed throttling.
- **DurationSec** `(int)` - `v26.3` `Optional`. The limit reverts to the previously active limit after this many seconds.

### Return value
Usually `true` but may return `false` if parameters were out of range.

### Remarks
- A value of `0` or negative returns the error `"Invalid parameter"`.
- A call without DurationSec cancels a pending revert.
- Calling with DurationSec while a timed limit is active keeps the original revert target and only updates the limit and the timer.
- The revert is checked once per minute.
