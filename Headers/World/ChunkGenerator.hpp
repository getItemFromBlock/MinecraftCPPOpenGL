#pragma once

#include <vector>
#include <atomic>
#include <thread>

#include <Perlin/PerlinNoise.hpp>

#include "Chunk.hpp"

namespace World
{
	class World;

	class ChunkGenerator
	{
	public:
		ChunkGenerator();
		~ChunkGenerator();

		std::atomic<bool> inputAccessFlag = false;
		std::vector<Chunk*> InputQueue;

		void InitThread(World* worldPtr);
		void ExitThread();
		void GenerateChunk(Chunk* input);
		unsigned int GetWorldSeed();

		float wFunc(int a, int b);
		float dFunc(int a, int b);
		float hFunc(int a, int b, int c);
	private:
		std::thread* generatorThread = nullptr;
		std::atomic<bool> shouldExit = false;
		std::vector<Chunk*> treatedChunks;
		unsigned int worldSeed;
		World* world = nullptr;

		void ThreadFunc();
		siv::BasicPerlinNoise<float> noiseA;
	};
}