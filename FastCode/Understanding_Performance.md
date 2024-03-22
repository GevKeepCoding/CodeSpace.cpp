# When talking about performance, what are the key metrics to consider?
* bandwidth, latency, and size.

### Definition of Latency:
* The time required to complete a single data transfer request. In mathematics
or science, this is also known as the "period" and we'll label it "T". The
inverse is the frequency, "f".

### Definition of Size(S):
The amount of data transferred in a single transfer request. Other terms used
for this include: record size,
block size, window size, etc. We'll label this "S".

### Definition of Bandwidth(B):
The total amount of data transferred per time unit(usually per second). Also
referred to as **maximum throughput**. I'll
label it "B". We can calculate this from "T"(or "f") and "S" as follows:
**B = S/T** or **B = S*f**.

Intuitive example:
Latency, T = 100ms or 0.1seconds.
This means, each data transfer takes 100ms, and in 1 second time, there can be
10 data transfers, which is the frequency. Mathematically f = 1/T = 1/0.1 = 10Hz.
Size, S = 10MB.
For each of data transfers, the payload is 10MB, *each* time.
Bandwidth, B = S/T = 10MB/0.1s = 100MB/s.

---