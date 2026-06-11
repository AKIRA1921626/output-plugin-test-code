// このサンプルのソースコードは自由に使用・改変・再配布して構いません。
// ただし、本プログラムの使用によるいかなる損害に対しても製作者は責任を負いません。
// また、本ソースコードはAIの助力を借用して作成しております。

#include <windows.h>
#include <stdio.h>
#include "output2.h"

bool func_output(OUTPUT_INFO* oip);
bool func_config(HWND hwnd, HINSTANCE dll_hinst) { return true; }
LPCWSTR func_get_config_text() { return L"統合テスト用"; }

OUTPUT_PLUGIN_TABLE output_plugin_table = {
	OUTPUT_PLUGIN_TABLE::FLAG_VIDEO,
	L"テスト出力プラグイン",
	L"Text File (*.txt)\0*.txt\0",
	L"AviUtl2 Tester Unified",
	func_output,
	func_config,
	func_get_config_text,
};

EXTERN_C __declspec(dllexport) bool InitializePlugin(DWORD version) { return true; }
EXTERN_C __declspec(dllexport) void UninitializePlugin() {}
EXTERN_C __declspec(dllexport) OUTPUT_PLUGIN_TABLE* GetOutputPluginTable(void) { return &output_plugin_table; }

//---------------------------------------------------------------------
//	デバッグ用出力関数（1ファイル統合・出力先指定版）
//---------------------------------------------------------------------
bool func_output(OUTPUT_INFO* oip) {
	// 1. ポインタのヌルチェック
	if (oip == nullptr) {
		MessageBoxW(NULL, L"oip is nullptr", L"Debug", MB_OK);
		return false;
	}

	// 2. ユーザーが指定した保存先（oip->savefile）で1つのファイルを作成
	// ワイド文字列の出力を安全に行うため、ccs=UTF-8 を指定して開く
	FILE* fp = nullptr;
	if (_wfopen_s(&fp, oip->savefile, L"w, ccs=UTF-8") != 0 || fp == nullptr) {
		MessageBoxW(NULL, L"エラー: 指定された場所にファイルを作成できませんでした。", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// 3. すべての情報をこの1つのファイルにまとめて出力
	// 安全のため、fprintf ではなくワイド文字列用の fwprintf_s を使用
	fwprintf_s(fp, L"=== AviUtl2 Pipeline Verification Success ===\n");
	fwprintf_s(fp, L"Output Path : %s\n", oip->savefile);
	fwprintf_s(fp, L"Width       : %d px\n", oip->w);
	fwprintf_s(fp, L"Height      : %d px\n", oip->h);
	fwprintf_s(fp, L"Frame Rate  : %.2f fps (rate:%d / scale:%d)\n", (double)oip->rate / oip->scale, oip->rate, oip->scale);
	fwprintf_s(fp, L"Total Frames: %d frames\n", oip->n);
	fwprintf_s(fp, L"Audio Rate  : %d Hz\n", oip->audio_rate);
	fwprintf_s(fp, L"Audio Ch    : %d ch\n", oip->audio_ch);

	// 確実に書き込みを完了させる
	fflush(fp);
	fclose(fp);

	// 4. 完了通知
	MessageBoxW(NULL, L"すべてのステップを通過し、指定された場所にファイルを1つ出力しました。", L"Debug", MB_OK);
	return true;
}