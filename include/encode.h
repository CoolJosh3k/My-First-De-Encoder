class Encoder {
	public:
		static bool SetInOutFileStrings(const char* in);
		static bool SetOutFileStrings(const char* out);
		static bool SetIter(char* const argv);
		static bool OpenFiles();
		static bool CloseFiles();
		static bool RenameOutFile();
		static bool Encode();
		
		#if DEBUG
			static void Print();
		#endif
};