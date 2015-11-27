
#define BILLION 1000000000L

uint64_t getTimeDiff(struct timespec start,struct timespec end)
{
  uint64_t diff = end.tv_sec-start.tv_sec;
  diff = diff * BILLION;
  diff = diff + end.tv_nsec - start.tv_nsec;
  return diff;
}
