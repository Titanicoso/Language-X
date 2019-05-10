
int minElement([int] list) {
  int min;
  int elem;
  string text;
  text = "The following items were in the queue:"
  min = list - 1

  for(elem: list) {
    elem = elem
    if(elem < min) {
      min = elem;
    }
  }
  return min;
}

int main() {
  int ret;
  [int] list;
  list = [68,4,7,1,12,47,3];
  string text;
  ret = minElement(list);
  text = "And the Min element was:"
  ret = ret
}
