#!/bin/bash
DIRNAME="$(dirname "$0")/../"
FILES=`find ${DIRNAME} -name "*.cpp" -or -name "*.hpp"`


OPTIONS="
--style=kr \
--indent=spaces=4 \
--indent-after-parens \
--indent-labels \
--indent-col1-comments \
--pad-oper \
--pad-comma \
--pad-header \
--convert-tabs \
--attach-closing-while \
--attach-return-type \
--attach-return-type-decl \
--add-braces \
--align-pointer=type \
--align-reference=type \
--keep-one-line-blocks \
--max-code-length=128 \
--suffix=none
"

astyle $OPTIONS $FILES
