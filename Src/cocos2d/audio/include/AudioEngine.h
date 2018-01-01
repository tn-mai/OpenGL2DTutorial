#pragma once

class AudioEngine
{
public:
  static void end() {}
  static void stopAll() {}
  static void uncache(const char*) {}
  static int play2d(const char*) { return 0; }
  static void setLoop(int, bool) {}
};