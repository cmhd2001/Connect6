#!/bin/bash

# Default variables
fichas=""
tiempo=0

# Parse command-line arguments
for arg in "$@"; do
    case $arg in
        -fichas=*)
            fichas="${arg#*=}"  # Extract value after '='
            shift
            ;;
        -tpj=*)
            tiempo="${arg#*=}"  # Extract value after '='
            shift
            ;;
        *)
            echo "Invalid option: $arg"
            exit 1
            ;;
    esac
done

# Validate arguments
if [[ -z "$fichas" || -z "$tiempo" ]]; then
    echo "Usage: ./agenteConnect6.sh -fichas=<blancas|negras> -tpj=<time_in_seconds>"
    exit 1
fi

# Run the Connect6 program with the parsed arguments
./connect6 --fichas="$fichas" --tiempo="$tiempo"