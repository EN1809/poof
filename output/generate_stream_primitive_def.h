
    
    struct primitive_def_stream_chunk
    {
      primitive_def Element;
      primitive_def_stream_chunk* Next;
    };

    
    struct primitive_def_stream
    {
      primitive_def_stream_chunk* FirstChunk;
      primitive_def_stream_chunk* LastChunk;
    };


    
    struct primitive_def_iterator
    {
      primitive_def_stream* Stream;
      primitive_def_stream_chunk* At;
    };

    function primitive_def_iterator
    Iterator(primitive_def_stream* Stream)
    {
      primitive_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(primitive_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(primitive_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(primitive_def_stream* Stream, primitive_def Element, memory_arena* Memory)
    {
      primitive_def_stream_chunk* NextChunk = (primitive_def_stream_chunk*)PushStruct(Memory, sizeof( primitive_def_stream_chunk ), 1, 0);
      NextChunk->Element = Element;

      if (!Stream->FirstChunk)
      {
        Assert(!Stream->LastChunk);
        Stream->FirstChunk = NextChunk;
        Stream->LastChunk = NextChunk;
      }
      else
      {
        Stream->LastChunk->Next = NextChunk;
        Stream->LastChunk = NextChunk;
      }

      Assert(NextChunk->Next == 0);
      Assert(Stream->LastChunk->Next == 0);

      return;
    }

    function void
    ConcatStreams( primitive_def_stream *S1, primitive_def_stream *S2)
    {
      if (S1->LastChunk)
      {
        S1->LastChunk->Next = S2->FirstChunk;
        S1->LastChunk = S2->LastChunk;
      }
      else
      {
        *S1 = *S2;
      }
    }


