<?hh

function handle_error($_errno, $str, ...) {
  echo " (error: $str)";
  return true;
}

class Foo { static function bar() {} }
class K { const A = 0; const B = 1; }
function LV($x) { return __hhvm_intrinsics\launder_value($x); }
function C(bool $b) {
  echo ($b ? " True\n" : " False\n");
}

function is_functions_static() {
  $m = class_meth(Foo::class, 'bar');

  echo 'is_array($m):'        ;C(is_array($m));
  echo 'hh\is_any_array($m):' ;C(hh\is_any_array($m));
  echo 'is_varray($m):'       ;C(is_varray($m));
  echo 'is_varray($m):'       ;C(is_varray($m));
  echo '$m is arraylike:'     ;C($m is arraylike);
  echo '$m is shape(...):'    ;C($m is shape(...));
  echo '$m is shape(str,str):';C($m is shape(K::A=>string, K::B=>string));

  echo '$m is HH\Traversable:'        ;C($m is HH\Traversable);
  echo '$m is Traversable:'           ;C($m is Traversable);
  echo '$m is KeyedTraversable:'      ;C($m is KeyedTraversable);
  echo '$m is HH\Rx\Traversable:'     ;C($m is HH\Rx\Traversable);
  echo '$m is HH\Rx\KeyedTraversable:';C($m is HH\Rx\KeyedTraversable);
  echo '$m is Container:'             ;C($m is Container);
  echo '$m is KeyedContainer:'        ;C($m is KeyedContainer);

  echo 'is_a($m, HH\Traversable):'        ;C(is_a($m, 'HH\Traversable'));
  echo 'is_a($m, Traversable):'           ;C(is_a($m, 'Traversable'));
  echo 'is_a($m, KeyedTraversable):'      ;C(is_a($m, 'KeyedTraversable'));
  echo 'is_a($m, HH\Rx\Traversable):'     ;C(is_a($m, 'HH\Rx\Traversable'));
  echo 'is_a($m, HH\Rx\KeyedTraversable):'
                                        ;C(is_a($m, 'HH\Rx\KeyedTraversable'));
  echo 'is_a($m, Container):'             ;C(is_a($m, 'Container'));
  echo 'is_a($m, KeyedContainer):'        ;C(is_a($m, 'KeyedContainer'));
}

function is_functions_dynamic() {
  $m = LV(class_meth(Foo::class, 'bar'));

  echo 'is_array($m):'        ;C(is_array($m));
  echo 'hh\is_any_array($m):' ;C(hh\is_any_array($m));
  echo 'is_varray($m):'       ;C(is_varray($m));
  echo 'is_varray($m):'       ;C(is_varray($m));
  echo '$m is arraylike:'     ;C($m is arraylike);
  echo '$m is shape(...):'    ;C($m is shape(...));
  echo '$m is shape(str,str):';C($m is shape(K::A=>string, K::B=>string));

  echo '$m is HH\Traversable:'        ;C($m is HH\Traversable);
  echo '$m is Traversable:'           ;C($m is Traversable);
  echo '$m is KeyedTraversable:'      ;C($m is KeyedTraversable);
  echo '$m is HH\Rx\Traversable:'     ;C($m is HH\Rx\Traversable);
  echo '$m is HH\Rx\KeyedTraversable:';C($m is HH\Rx\KeyedTraversable);
  echo '$m is Container:'             ;C($m is Container);
  echo '$m is KeyedContainer:'        ;C($m is KeyedContainer);

  echo 'is_a($m, HH\Traversable):'   ;C(LV('is_a')($m, 'HH\Traversable'));
  echo 'is_a($m, Traversable):'      ;C(LV('is_a')($m, 'Traversable'));
  echo 'is_a($m, KeyedTraversable):' ;C(LV('is_a')($m, 'KeyedTraversable'));
  echo 'is_a($m, HH\Rx\Traversable):';C(LV('is_a')($m, 'HH\Rx\Traversable'));
  echo 'is_a($m, HH\Rx\KeyedTraversable):'
                                  ;C(LV('is_a')($m, 'HH\Rx\KeyedTraversable'));
  echo 'is_a($m, Container):'        ;C(LV('is_a')($m, 'Container'));
  echo 'is_a($m, KeyedContainer):'   ;C(LV('is_a')($m, 'KeyedContainer'));
}

function is_functions_builtins() {
  $m = LV(class_meth(Foo::class, 'bar'));

  echo 'is_array($m):'       ;C(LV('is_array')($m));
  echo 'hh\is_any_array($m):';C(LV('hh\is_any_array')($m));
  echo 'hh\is_varray($m):'   ;C(LV('hh\is_varray')($m));
}

<<__EntryPoint>>
function main() {
  set_error_handler('handle_error');

  is_functions_static();
  is_functions_dynamic();
  is_functions_builtins();
}
