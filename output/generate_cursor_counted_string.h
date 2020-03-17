

    struct counted_string_cursor
    {
      counted_string* Start;
      counted_string* End;
      counted_string* At;
    };

    function counted_string_cursor
    CountedStringCursor(umm ElementCount, memory_arena* Memory)
    {
      counted_string* Start = (counted_string*)PushStruct(Memory, sizeof(counted_string), 1, 1);
      counted_string_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    };


