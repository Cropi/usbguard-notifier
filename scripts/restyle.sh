#!/bin/bash
DIRNAME="$(dirname "$0")/../"
FILES=`find ${DIRNAME} -name "*.cpp" -or -name "*.hpp"`


OPTIONS="
--style=kr \
--indent=spaces=4 \
--indent-namespaces \
--indent-after-parens \
--indent-labels \
--indent-col1-comments \
--convert-tabs \
--attach-closing-while \
--attach-return-type-decl \
--break-one-line-headers \
--add-braces \
--align-pointer=type \
--align-reference=type \
--max-code-length=128 \
--suffix=none
"

astyle $OPTIONS $FILES
