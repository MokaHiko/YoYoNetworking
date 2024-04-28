#include <gtest/gtest.h>

#include <MemoryStream.h>

TEST(OutputStreamTest, BasicAssertions)
{
	OutputMemoryStream out_stream = {};

	{
		char data[16];
		memset(data, 0, 16);

		out_stream.Write(data, 16);
		EXPECT_EQ(out_stream.GetBufferLength(), 16);
	}

	{
		char data[16];
		memset(data, 0, 16);

		out_stream.Write(data, 16);
		EXPECT_EQ(out_stream.GetBufferLength(), 32);
	}
}

TEST(InputStreamTest, BasicAssertions)
{
	{
		char* data = static_cast<char*>(malloc(32));
		memset(data, 0, 32);

		InputMemoryStream input_stream(data, 32);
		EXPECT_EQ(input_stream.GetRemainingBytesSize(), 32);

		char read_buffer[32];
		memset(read_buffer, 0, 32);
		input_stream.Read(read_buffer, 32);
		EXPECT_EQ(input_stream.GetRemainingBytesSize(), 0);
	}

	{
		char nums[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '\0'};
		char* data = static_cast<char*>(malloc(16));
		memcpy(data, nums, 16);

		InputMemoryStream input_stream(data, 16);

		char output[16] = {0};

		int ctr = 0;
		while(input_stream.GetRemainingBytesSize() > 0)
		{
			char buffer = '0';
			input_stream.Read(&buffer, 1);
			output[ctr++] = buffer;
		}

		EXPECT_EQ(input_stream.GetRemainingBytesSize(), 0);
		EXPECT_TRUE(strcmp(output, nums) == 0);
	}
}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}