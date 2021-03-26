; ModuleID = 'test1.cpp'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZL1d = internal global i32 0, align 4

; Function Attrs: nounwind uwtable
define i32 @_Z5shiftii(i32 %c, i32 %b) #0 {
entry:
  %c.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 %c, i32* %c.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  store i32 12, i32* %a, align 4
  %0 = load i32* %a, align 4
  %shl = shl i32 %0, 4
  store i32 %shl, i32* @_ZL1d, align 4
  %1 = load i32* %c.addr, align 4
  %2 = load i32* %b.addr, align 4
  %sub = sub nsw i32 %1, %2
  %3 = load i32* @_ZL1d, align 4
  %add = add nsw i32 %3, %sub
  store i32 %add, i32* @_ZL1d, align 4
  %4 = load i32* @_ZL1d, align 4
  ret i32 %4
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.6.2 (tags/RELEASE_362/final)"}
