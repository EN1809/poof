link_internal ui_toggle_linked_list_node *
Allocate_ui_toggle_linked_list_node(memory_arena *Memory)
{
  ui_toggle_linked_list_node *Result = Allocate( ui_toggle_linked_list_node, Memory, 1);
  return Result;
}

link_internal ui_toggle_hashtable
Allocate_ui_toggle_hashtable(umm ElementCount, memory_arena *Memory)
{
  ui_toggle_hashtable Result = {
    .Elements = Allocate( ui_toggle_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal ui_toggle_linked_list_node *
GetHashBucket(umm HashValue, ui_toggle_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  ui_toggle_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal ui_toggle *
GetFirstAtBucket(umm HashValue, ui_toggle_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  ui_toggle_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  ui_toggle *Result = &Bucket->Element;
  return Result;
}

link_internal ui_toggle *
Insert(ui_toggle_linked_list_node *Node, ui_toggle_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  ui_toggle_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket) Bucket = &(*Bucket)->Next;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal ui_toggle*
Insert(ui_toggle Element, ui_toggle_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  ui_toggle_linked_list_node *Bucket = Allocate_ui_toggle_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

//
// Iterator impl.
//
link_inline umm
ZerothIndex(ui_toggle_hashtable *Hashtable)
{
  return 0;
}

