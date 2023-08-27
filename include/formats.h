static const int ITER_BITS_POS {2};
static const int MODE_BITS_POS {6};
static const uint8_t MODE_BITS_MASK {0b11 << MODE_BITS_POS};
static const int PAD_BITS_POS {0};
static const uint8_t HEADER_SIZE {1};

enum Mode : char {
	mode_2 = 0b1,
	mode_8 = 0b10
};

const struct Ext {
	const std::string done {".rle"};
	const std::string temp {".tmp"};
	const std::string done_temp {done+temp};
} ext;