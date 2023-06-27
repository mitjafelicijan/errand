{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [
      gcc
      clang
      tinycc
      musl
      bash
      zsh
      fish
    ];
}
