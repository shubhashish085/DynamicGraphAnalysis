#include "MatchingCommand.h"

MatchingCommand::MatchingCommand(const int argc, char **argv) : CommandParser(argc, argv) {
    // Initialize options value
    options_key[OptionKeyword::DataGraphFile] = "-d";
    options_key[OptionKeyword::OutputFilePath] = "-o";
    processOptions();
};

void MatchingCommand::processOptions() {

    options_value[OptionKeyword::DataGraphFile] = getCommandOption(options_key[OptionKeyword::DataGraphFile]);
    options_value[OptionKeyword::OutputFilePath] = getCommandOption(options_key[OptionKeyword::OutputFilePath]);
}