#include <stdio.h>
#include <stdlib.h>
#include "sddapi.h"

int main(int argc, char** argv) {

  // set up vtree and manager
  Vtree* vtree = sdd_vtree_read("input/big-swap.vtree");
  SddManager* manager = sdd_manager_new(vtree);

  printf("reading sdd from file ...\n");
  SddNode* alpha = sdd_read("input/big-swap.sdd",manager);
  printf("  sdd size = %zu\n", sdd_size(alpha));

  // to perform a swap, we need the manager's vtree
  Vtree* manager_vtree = sdd_manager_vtree(manager);
  double limit;

  // ref alpha (no dead nodes when swapping)
  sdd_ref(alpha,manager);

  limit = 2.0;
  printf("modifying vtree (swap node 7) (limit growth by %.1fx) ... ",limit);
  int succeeded;
  sdd_manager_set_size_limit_context(manager_vtree,manager);
  succeeded = sdd_vtree_swap(manager_vtree,manager,0,limit,0);
  printf("%s!\n", succeeded?"succeeded":"did not succeed");
  printf("  sdd size = %zu\n", sdd_size(alpha));

  printf("modifying vtree (swap node 7) (no limit) ... ");
  succeeded = sdd_vtree_swap(manager_vtree,manager,0,0.0,0);
  printf("%s!\n", succeeded?"succeeded":"did not succeed");
  printf("  sdd size = %zu\n", sdd_size(alpha));

  printf("updating context of size limit ...\n");
  sdd_manager_update_size_limit_context(manager);

  Vtree* left_vtree = sdd_vtree_left(manager_vtree);
  limit = 1.2;
  printf("modifying vtree (swap node 5) (limit growth by %.1fx) ... ",limit);
  succeeded = sdd_vtree_swap(left_vtree,manager,0,limit,0);
  printf("%s!\n", succeeded?"succeeded":"did not succeed");
  printf("  sdd size = %zu\n", sdd_size(alpha));

  limit = 1.3;
  printf("modifying vtree (swap node 5) (limit growth by %.1fx) ... ",limit);
  succeeded = sdd_vtree_swap(left_vtree,manager,0,limit,0);
  printf("%s!\n", succeeded?"succeeded":"did not succeed");
  printf("  sdd size = %zu\n", sdd_size(alpha));

  // deref alpha, since ref's are no longer needed
  sdd_deref(alpha,manager);

  sdd_vtree_free(vtree);
  sdd_manager_free(manager);

  return 0;
}
