        struct stl_container_def_stream_chunk
    {
      stl_container_def Element;
      stl_container_def_stream_chunk* Next;
    };

        struct stl_container_def_stream
    {
      stl_container_def_stream_chunk* FirstChunk;
      stl_container_def_stream_chunk* LastChunk;
    };


        struct stl_container_def_iterator
    {
      stl_container_def_stream* Stream;
      stl_container_def_stream_chunk* At;
    };

    bonsai_function stl_container_def_iterator
    Iterator(stl_container_def_stream* Stream)
    {
      stl_container_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(stl_container_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(stl_container_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }

    bonsai_function b32
    IsLastElement(stl_container_def_iterator* Iter)
    {
      b32 Result = Iter->At->Next == 0;
      return Result;
    }


        bonsai_function stl_container_def *
    Push(stl_container_def_stream* Stream, stl_container_def Element, memory_arena* Memory)
    {
      stl_container_def_stream_chunk* NextChunk = (stl_container_def_stream_chunk*)PushStruct(Memory, sizeof( stl_container_def_stream_chunk ), 1, 0);
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

      stl_container_def *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( stl_container_def_stream *S1, stl_container_def_stream *S2)
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


