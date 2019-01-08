#include <bits/stdc++.h>
using namespace std;

// Check kernel/time/clocksource.c for details.
// This algorithm has been stable for both linux-3.X.X and linux-4.X.X versions

using u32 = uint32_t;
using u64 = uint64_t;

void clocks_calc_mult_shift(u32* mult, u32* shift, u32 from, u32 to, u32 maxsec) {
  u64 tmp;
  u32 sft, sftacc = 32;

  /*
   *    * Calculate the shift factor which is limiting the conversion
   *       * range:
   *          */
  tmp = ((u64)maxsec * from) >> 32;
  while (tmp) {
    tmp >>= 1;
    sftacc--;
  }

  /*
   *    * Find the conversion shift/mult pair which has the best
   *       * accuracy and fits the maxsec conversion range:
   *          */
  for (sft = 32; sft > 0; sft--) {
    tmp = (u64)to << sft;
    tmp += from / 2;
    // do_div(tmp, from);
    tmp /= from;
    if ((tmp >> sftacc) == 0) break;
  }
  *mult = tmp;
  *shift = sft;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "usage: " << argv[0] << " tsc_khz" << endl;
    cout << endl
         << "use: dmesg | grep \"tsc: Refined TSC clocksource calibration\" to get tsc_mhz, and remove the dot to get "
            "tsc_khz"
         << endl;
    exit(1);
  }
  u32 freq = stoi(argv[1]);
  u32 mult, shift;
  u64 sec = 600;
  clocks_calc_mult_shift(&mult, &shift, freq, 1000000, sec * 1000);
  double tsc_ghz = 1.0 / mult * (1 << shift);
  cout << std::setprecision(17) << "tsc_ghz: " << tsc_ghz << endl;
  return 0;
}
