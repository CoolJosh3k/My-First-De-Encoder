class Decoder {
	public:
		static bool SetInOutFileStrings(char* const in, char* const out);
		static bool OpenFiles();
		static bool CloseFiles();
		static bool Decode();
};