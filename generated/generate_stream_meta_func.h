        struct meta_func_stream_chunk
    {
      meta_func Element;
      meta_func_stream_chunk* Next;
    };

        struct meta_func_stream
    {
      meta_func_stream_chunk* FirstChunk;
      meta_func_stream_chunk* LastChunk;
    };


        struct meta_func_iterator
    {
      meta_func_stream* Stream;
      meta_func_stream_chunk* At;
    };

    link_internal meta_func_iterator
    Iterator(meta_func_stream* Stream)
    {
      meta_func_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    link_internal b32
    IsValid(meta_func_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    link_internal void
    Advance(meta_func_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }

    link_internal b32
    IsLastElement(meta_func_iterator* Iter)
    {
      b32 Result = Iter->At->Next == 0;
      return Result;
    }


        link_internal meta_func *
    Push(meta_func_stream* Stream, meta_func Element, memory_arena* Memory)
    {
      meta_func_stream_chunk* NextChunk = (meta_func_stream_chunk*)PushStruct(Memory, sizeof(meta_func_stream_chunk), 1, 0);
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

      meta_func *Result = &NextChunk->Element;
      return Result;
    }

    link_internal void
    ConcatStreams( meta_func_stream *S1, meta_func_stream *S2)
    {
      if (S1->LastChunk)
      {
        Assert(S1->FirstChunk);

        if (S2->FirstChunk)
        {
          Assert(S2->LastChunk);
          S1->LastChunk->Next = S2->FirstChunk;
          S1->LastChunk = S2->LastChunk;
        }
        else
        {
          Assert(!S2->LastChunk);
        }
      }
      else
      {
        Assert(!S1->FirstChunk);
        Assert(!S1->LastChunk);

        if(S2->FirstChunk)
        {
          Assert(S2->LastChunk);
        }
        else
        {
          Assert(!S2->LastChunk);
        }

        *S1 = *S2;
      }
    }


