void test_simple_bad(int *p) {
  int x;
  x = *p;
  if (p == nullptr) { // BAD
    return;
  }
}

void test_not_same_basic_block(int *p) {
  int x = *p;
  if (x > 100)
    return;
  if (!p) // BAD
    return;
}

void test_indirect(int **p) {
  int x;
  x = **p;
  if (*p == nullptr) { // BAD [NOT DETECTED]
    return;
  }
}

struct ContainsIntPtr {
  int **intPtr;
};

bool check_curslist(ContainsIntPtr *cip) {
  // both the deref and the null check come from the same instruction, but it's
  // an AliasedDefinition instruction.
  return *cip->intPtr != nullptr; // GOOD
}

void test_no_single_dominator(int *p, bool b) {
  int x;
  if (b) {
    x = *p;
  } else {
    x = *p;
  }
  if (p == nullptr) { // BAD [NOT DETECTED]
    return;
  }
}

int test_postdominator_same_bb(int *p) {
  int b = (p == nullptr); // BAD
  // This dereference is a postdominator of the null check, meaning that all
  // paths from the check to the function exit will pass through it.
  return *p + b;
}

int test_postdominator(int *p) {
  int b = (p == nullptr); // BAD [NOT DETECTED]

  if (b) b++; // This line breaks up the basic block

  // This dereference is a postdominator of the null check, meaning that all
  // paths from the check to the function exit will pass through it.
  return *p + b;
}

int test_inverted_logic(int *p) {
  if (p == nullptr) { // BAD [NOT DETECTED]
    // The check above should probably have been `!=` instead of `==`.
    return *p;
  } else {
    return 0;
  }
}
