#include "SerialInstruction.hpp"

void sendPacket(const Command& command, Serial& serial) {
	// フレームの開始と終了を示すマーカー
	const String START_MARKER = U"<START>";
	const String END_MARKER = U"<END>";

	// コマンドにマーカーを付けてパケットを作成
	String packet = U"{}{},{}{}"_fmt(START_MARKER, command.buttonByte, command.repeatTimes, END_MARKER);
	Console << U"パケット: {}を送信します"_fmt(packet);
	// パケットを送信
	Console << serial.write(packet.data(), packet.size());
}
