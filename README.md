# xprdatabench

The CPU benchmark contract behind [xprdata.org/benchmark](https://xprdata.org/benchmark), which shows how much CPU time each active XPR Network block producer bills for the same transaction.

## What the contract does

One action, `cpu(uint64 nonce, uint32 limit)`. It counts the primes below `limit` by trial division and prints the count, so the compiler cannot optimize the loop away. `nonce` only makes every transaction unique. Only the contract account itself can call the action, and `limit` is capped at 100,000.

On mainnet the benchmark calls it with `limit = 3200`. At that workload the network median lands on the same scale as the `protobench` benchmark shown by Saltant (control measurement on 28 Sep 2026: 0.226 ms here, 0.234 ms for `protobench`).

## How the measurement works

- A worker sends one `cpu` transaction on average every 3 seconds, at random intervals, so every position in a producer's slot gets sampled.
- The producer that puts the transaction into its block records the CPU time it billed (`cpu_usage_us`) in the block's transaction receipt.
- The worker reads that number, and the block's producer, from irreversible blocks only.
- Transactions are signed with a dedicated `bench` permission that is linked to this one action (`linkauth`). The key on the server cannot transfer tokens or change the contract.

## Verify the deployed code

The build is reproducible. With CDT 4.1.1:

```bash
cdt-cpp -abigen -o xprdatabench.wasm xprdatabench.cpp
sha256sum xprdatabench.wasm
```

gives `6fab546c9e2c420e687a3babb6179c9ce2b750c53e1af2e3d1bf7fcd6fe36808`, the same hash as the code deployed on chain:

```bash
curl -s -X POST https://mainnet-api.xprdata.org/v1/chain/get_code_hash \
  -d '{"account_name":"xprdatabench"}'
```

## Data

Every measurement is public. Use "Download JSON" on [xprdata.org/benchmark](https://xprdata.org/benchmark) to get the raw samples.

## License

MIT
