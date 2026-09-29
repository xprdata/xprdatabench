#include <eosio/eosio.hpp>
#include <eosio/print.hpp>

using namespace eosio;

class [[eosio::contract("xprdatabench")]] xprdatabench : public contract {
public:
   using contract::contract;

   // CPU benchmark: count primes below `limit` via trial division.
   // `nonce` only makes each transaction unique (prevents duplicate-tx rejection).
   [[eosio::action]]
   void cpu(uint64_t nonce, uint32_t limit) {
      require_auth(get_self());
      check(limit >= 2 && limit <= 100000, "limit out of range");

      uint32_t count = 0;
      for (uint32_t n = 2; n < limit; ++n) {
         bool prime = true;
         for (uint32_t d = 2; d * d <= n; ++d) {
            if (n % d == 0) { prime = false; break; }
         }
         if (prime) ++count;
      }

      // Pass result to a host function so the optimizer cannot drop the loop.
      print(count);
   }
};
